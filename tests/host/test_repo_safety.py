"""Data-free regression checks for the source/recipe repository guard."""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]


class RepositorySafetyTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix='sonicpad-safety-')
        self.addCleanup(self.temporary.cleanup)
        self.root = Path(self.temporary.name)
        (self.root / 'scripts').mkdir()
        (self.root / 'config').mkdir()
        for name in ('.gitignore', 'scripts/check-repo-safety.sh'):
            shutil.copy2(ROOT / name, self.root / name)
        (self.root / 'config/recomp-blocks.txt').write_text(
            '# Synthetic metadata, no game content\n'
            '@0 TEST.BIN 0123456789abcdef\n'
            '0C010000 2 2 @0 0\n')
        (self.root / 'config/code-modules.txt').write_text(
            'SONICAD/TEST.PRS 0C900000 0123456789abcdef\n')
        self.git('init', '-q')
        self.git('add', '.')
        self.git('-c', 'user.name=Safety Test', '-c', 'user.email=safety@example.invalid',
                 '-c', 'commit.gpgsign=false', 'commit', '-qm', 'Synthetic fixture')

    def git(self, *args):
        return subprocess.run(['git', *args], cwd=self.root, check=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    def check(self):
        return subprocess.run(['sh', 'scripts/check-repo-safety.sh'], cwd=self.root,
                              stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)

    def test_metadata_only_tree_passes(self):
        result = self.check()
        self.assertEqual(result.returncode, 0, result.stdout)

    def test_instruction_column_is_rejected(self):
        with (self.root / 'config/recomp-blocks.txt').open('a') as stream:
            stream.write('0C010004 2 2 @0 4 00090009\n')
        self.assertNotEqual(self.check().returncode, 0)

    def test_malformed_five_column_row_is_rejected(self):
        with (self.root / 'config/recomp-blocks.txt').open('a') as stream:
            stream.write('not-an-address 2 2 @0 4\n')
        self.assertNotEqual(self.check().returncode, 0)

    def test_module_bytes_column_is_rejected(self):
        with (self.root / 'config/code-modules.txt').open('a') as stream:
            stream.write('SONICAD/TEST2.PRS 0C900000 0123456789abcdef 00090009\n')
        self.assertNotEqual(self.check().returncode, 0)

    def test_private_artifacts_are_ignored_and_force_added_files_fail(self):
        for name in ('personal.ipa', 'save.vms', 'save.vmi', 'identity.p12', '.env'):
            with self.subTest(name=name):
                path = self.root / name
                path.write_text('synthetic fixture')
                self.git('check-ignore', '-q', name)
                self.git('add', '-f', name)
                self.assertNotEqual(self.check().returncode, 0)
                self.git('rm', '--cached', '--', name)


if __name__ == '__main__':
    unittest.main()
