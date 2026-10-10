"""Independent byte-stream fixtures; no game data."""
import importlib.util
from pathlib import Path
import unittest

spec = importlib.util.spec_from_file_location("prs", Path(__file__).resolve().parents[2] / "scripts/decode-recovery-prs.py")
prs = importlib.util.module_from_spec(spec)
spec.loader.exec_module(prs)


class PRSTests(unittest.TestCase):
    def test_literals_and_terminator(self):
        self.assertEqual(prs.decode_prs(b"\x17ABC\0\0"), (b"ABC", 6))

    def test_short_overlapping_copy(self):
        # literal A; short copy length 5, offset -1; terminator.
        self.assertEqual(prs.decode_prs(bytes([0x59, 65, 255, 0, 0])), (b"AAAAAA", 5))

    def test_long_copy(self):
        self.assertEqual(prs.decode_prs(bytes([0x15, 65, 0xFF, 0xFF, 0, 0])), (b"A" * 10, 6))

    def test_extended_length(self):
        self.assertEqual(prs.decode_prs(bytes([0x15, 65, 0xF8, 0xFF, 19, 0, 0])), (b"A" * 21, 7))

    def test_control_reload(self):
        self.assertEqual(prs.decode_prs(b"\xFF12345678\x02\0\0"), (b"12345678", 12))

    def test_truncated(self):
        for data in [b"", b"\x01", b"\x02\0"]:
            with self.assertRaisesRegex(ValueError, "Truncated"):
                prs.decode_prs(data)

    def test_invalid_backreference(self):
        with self.assertRaisesRegex(ValueError, "precedes"):
            prs.decode_prs(bytes([0, 255]))

    def test_output_limit(self):
        with self.assertRaisesRegex(ValueError, "limit"):
            prs.decode_prs(bytes([0x15, 65, 0xF8, 0xFF, 19, 0, 0]), limit=20)


if __name__ == "__main__":
    unittest.main()
