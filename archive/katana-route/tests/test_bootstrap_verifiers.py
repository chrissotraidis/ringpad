"""Synthetic fixtures exercise verifier failure contracts, never real game data."""
import hashlib
import importlib.util
import io
from pathlib import Path
import tarfile
import tempfile
import unittest
from unittest import mock
import zipfile

ROOT = Path(__file__).resolve().parents[2]


def module(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    result = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(result)
    return result


game = module('verify_game', ROOT / 'scripts/verify-game.py')
sources = module('source_audit', ROOT / 'scripts/check-source-closure.py')


class GameInputTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.expected = []
        self.lines = ['3']
        for n, lba, kind in ((1, 0, 4), (2, 11511, 0), (3, 45000, 4)):
            data = f'SYNTHETIC-TRACK-{n}'.encode()
            name = f'track {n}.bin'
            (self.root / name).write_bytes(data)
            self.expected.append(dict(n=n, lba=lba, type=kind, sector_size=2352,
                                      bytes=len(data), sha256=hashlib.sha256(data).hexdigest()))
            self.lines.append(f'{n} {lba} {kind} 2352 "{name}" 0')
        self.gdi = self.root / 'fixture.gdi'
        self.write_descriptor()

    def write_descriptor(self):
        self.gdi.write_text('\n'.join(self.lines) + '\n')

    def test_valid_quoted_names_and_streamed_hashes(self):
        result = game.verify_gdi(self.gdi, self.expected)
        self.assertEqual(len(result['tracks']), 3)
        self.assertEqual(result['descriptor_sha256'], game.digest(self.gdi))

    def test_same_size_wrong_content_rejected(self):
        file = self.root / 'track 3.bin'
        file.write_bytes(b'x' * file.stat().st_size)
        with self.assertRaisesRegex(ValueError, 'SHA-256 mismatch'):
            game.verify_gdi(self.gdi, self.expected)

    def test_missing_track_rejected(self):
        (self.root / 'track 2.bin').unlink()
        with self.assertRaisesRegex(ValueError, 'Track missing'):
            game.verify_gdi(self.gdi, self.expected)

    def test_wrong_offset_and_revision_layout_rejected(self):
        self.lines[3] = self.lines[3][:-1] + '16'
        self.write_descriptor()
        with self.assertRaisesRegex(ValueError, 'Unsupported layout'):
            game.verify_gdi(self.gdi, self.expected)

    def test_extra_track_rejected(self):
        self.lines.append('4 50000 4 2352 other.bin 0')
        self.write_descriptor()
        with self.assertRaisesRegex(ValueError, 'exactly three'):
            game.verify_gdi(self.gdi, self.expected)

    def test_path_escape_rejected(self):
        self.lines[1] = '1 0 4 2352 ../external.bin 0'
        self.write_descriptor()
        with self.assertRaisesRegex(ValueError, 'selected directory'):
            game.verify_gdi(self.gdi, self.expected)

    def test_symlink_escape_rejected(self):
        with tempfile.TemporaryDirectory() as outside:
            external = Path(outside) / 'external.bin'
            external.write_bytes(b'SYNTHETIC-TRACK-1')
            link = self.root / 'escape.bin'
            link.symlink_to(external)
            self.lines[1] = '1 0 4 2352 escape.bin 0'
            self.write_descriptor()
            with self.assertRaisesRegex(ValueError, 'outside selected directory'):
                game.verify_gdi(self.gdi, self.expected)

    def test_cdi_not_treated_as_supported_disc(self):
        with self.assertRaisesRegex(ValueError, 'not CDI'):
            game.verify_gdi(self.root / 'unsupported.cdi', self.expected)


class SourceInputTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def test_windows_library_does_not_supply_missing_source(self):
        (self.root / 'katana_generated.lib').write_bytes(b'WINDOWS-OBJECT-FIXTURE')
        report = sources.check_file(self.root, 'unit.cpp', 4, hashlib.sha256(b'code').hexdigest())
        self.assertEqual(report['status'], 'missing')

    def test_present_source_must_match_bytes_not_filename(self):
        (self.root / 'unit.cpp').write_bytes(b'evil')
        report = sources.check_file(self.root, 'unit.cpp', 4, hashlib.sha256(b'code').hexdigest())
        self.assertEqual(report['status'], 'incompatible')

    def test_runtime_zip_alone_not_materialized_source(self):
        archive = self.root / 'katana-source-test.zip'
        with zipfile.ZipFile(archive, 'w') as package:
            package.writestr('src/runtime/native_port.cpp', b'code')
        record = dict(path=sources.BASE_PREFIX + archive.name,
                      bytes=archive.stat().st_size, sha256=game.digest(archive))
        result = sources.runtime_archive(self.root, self.root / 'runtime', record)
        self.assertEqual(result['verified'], 1)
        self.assertEqual(result['missing'], 1)

    def test_runtime_source_changes_detected_even_with_valid_zip(self):
        archive = self.root / 'katana-source-test.zip'
        with zipfile.ZipFile(archive, 'w') as package:
            package.writestr('native.cpp', b'code')
        (self.root / 'native.cpp').write_bytes(b'evil')
        record = dict(path=sources.BASE_PREFIX + archive.name,
                      bytes=archive.stat().st_size, sha256=game.digest(archive))
        result = sources.runtime_archive(self.root, self.root, record)
        self.assertEqual(result['incompatible'], 1)

    def test_changed_ffmpeg_source_not_hidden_by_cache_stamp(self):
        cache = self.root / '.local/media-source'
        cache.mkdir(parents=True)
        archive = cache / 'ffmpeg-fixture.tar.gz'
        with tarfile.open(archive, 'w:gz') as package:
            member = tarfile.TarInfo('FFmpeg-fixture/decoder.c')
            member.size = 4
            package.addfile(member, io.BytesIO(b'code'))
        source = cache / 'FFmpeg-fixture'
        source.mkdir()
        (source / 'decoder.c').write_bytes(b'evil')
        (source / '.ringpad-source-archive-sha256').write_text(game.digest(archive))
        with mock.patch.object(sources, 'ROOT', self.root):
            report = sources.media_source(dict(revision='fixture', archive_sha256=game.digest(archive)))
        self.assertEqual(report['incompatible'], 1)

    def test_archive_path_escape_rejected(self):
        for path in ('../evil.cpp', '/evil.cpp', 'C:/evil.cpp'):
            with self.subTest(path=path), self.assertRaises(ValueError):
                sources.safe_relative(path)

    def test_lock_drift_rejected(self):
        pins = {'references': [{'name': 'SDK', 'pin': 'expected'}]}
        lock = {'references': {'SDK': {'commit': 'different'}},
                'media_source': {'commit': 'media', 'archive_sha256': 'hash'}}
        media = {'revision': 'media', 'archive_sha256': 'hash'}
        with self.assertRaisesRegex(ValueError, 'lock disagrees with SDK'):
            sources.validate_lock(pins, lock, media)

    def test_ffmpeg_pin_drift_rejected(self):
        lock = {'references': {}, 'media_source': {'commit': 'media', 'archive_sha256': 'hash'}}
        with self.assertRaisesRegex(ValueError, 'lock disagrees with FFmpeg'):
            sources.validate_lock({'references': []}, lock,
                                  {'revision': 'other', 'archive_sha256': 'hash'})


if __name__ == '__main__':
    unittest.main()
