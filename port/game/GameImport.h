#pragma once

/* RingPad game import: verify an owner-supplied Sonic Adventure GDI.
 *
 * Only the PAL v1.003 (MK-5100050) GDI with its three tracks is accepted. The
 * track contents decide compatibility: layout, sizes and SHA-256 must match
 * config/game-identity.json. The descriptor's own hash is recorded but not
 * required, since equivalent GDI text can differ in whitespace. Pure C++ with
 * CommonCrypto for hashing, so it runs in host tests and in the iOS app. */

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace ringpad {

struct TrackIdentity {
    int number;
    int lba;
    int type;
    int sector_size;
    uint64_t bytes;
    const char *sha256;
};

struct GameIdentity {
    const char *release;
    const char *product;
    const char *gdi_sha256;
    std::array<TrackIdentity, 3> tracks;
};

/* Sonic Adventure PAL v1.003 (1999), MK-5100050. */
const GameIdentity& supported_game() noexcept;

struct GdiTrack {
    int number = 0, lba = 0, type = 0, sector_size = 0, offset = 0;
    std::string filename;
};

/* Parse GDI text. Accepts quoted filenames; rejects paths and traversal. */
bool parse_gdi(const std::string& text, std::vector<GdiTrack>& out, std::string& error);

struct GameCheck {
    bool ok = false;
    std::string error;
    std::string gdi_path;                 /* the descriptor that was checked */
    std::string gdi_sha256;
    std::vector<std::string> track_paths; /* in track order */
};

using HashProgress = std::function<void(uint64_t done, uint64_t total)>;

/* Hash a file with SHA-256 (lowercase hex). Returns "" on read failure. */
std::string sha256_file(const std::string& path, const HashProgress& progress = {},
                        uint64_t base = 0, uint64_t total = 0);

/* Verify a folder holding exactly one .gdi and its tracks, or a .gdi path. */
GameCheck verify_game(const std::string& folder_or_gdi, const GameIdentity& identity,
                      const HashProgress& progress = {});

} /* namespace ringpad */
