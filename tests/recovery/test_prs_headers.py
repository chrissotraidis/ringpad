"""The classifier recognizes structure, without declaring other bytes non-code."""
import importlib.util
from pathlib import Path
import struct
import unittest

spec = importlib.util.spec_from_file_location("scan", Path(__file__).resolve().parents[2] / "scripts/scan-recovery-prs.py")
scan = importlib.util.module_from_spec(spec)
spec.loader.exec_module(scan)


class HeaderTests(unittest.TestCase):
    def sample(self, base):
        data = bytearray(512)
        struct.pack_into("<4H", data, 0, 0xD308, 0xD507, 0x432B, 0xE600)
        struct.pack_into("<II", data, 32, (base & 0x1FFFFFFF) + 64, 0x8C010100)
        struct.pack_into("<6I", data, 64, 12, 2, base + 128, base + 144, base + 160, base + 176)
        return data

    def test_relocated_module(self):
        for base in (0x8C900000, 0x8CB80000):
            header = scan.module_header(self.sample(base), {0x8C010100})
            self.assertTrue(header["resident_entry_known"])
            self.assertEqual([h["base"] for h in header["placement_hypotheses"]], [f"0x{base:08X}"])

    def test_bad_callback_not_accepted(self):
        data = self.sample(0x8C900000)
        struct.pack_into("<I", data, 72, 0x8C900081)
        self.assertEqual(scan.module_header(data, set())["placement_hypotheses"], [])

    def test_header_literal_outside_image(self):
        data = self.sample(0x8C900000)
        struct.pack_into("<H", data, 0, 0xD3FF)
        self.assertIn("error", scan.module_header(data, set()))

    def test_prefixes_keep_unknowns(self):
        self.assertEqual(scan.prefix_kind(b"PVMH", None), "PVMH_prefix")
        self.assertEqual(scan.prefix_kind(bytes.fromhex("e62f"), None), "stack_prologue_prefix")
        self.assertEqual(scan.prefix_kind(bytes.fromhex("224f"), None), "stack_prologue_prefix")
        self.assertEqual(scan.prefix_kind(b"", None), "other_prefix")
        self.assertIsNone(scan.module_header(b"PVMH" + bytes(40), set()))


if __name__ == "__main__":
    unittest.main()
