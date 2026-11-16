/* RingPad game import verifier. See GameImport.h. */

#include "GameImport.h"

#include <CommonCrypto/CommonDigest.h>

#include <cctype>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

namespace ringpad {

const GameIdentity& supported_game() noexcept {
    static const GameIdentity identity{
        "Sonic Adventure PAL v1.003 (1999)",
        "MK-5100050",
        "d6e475056e725ae2887f613d4788c2376f86630227c19404d1436ac0672a2d1e",
        {{
            {1, 0, 4, 2352, 26721072ull, "cb45a8de12af5b13abb559fc8acc714744a4e22965fe67158cdbd0037cace093"},
            {2, 11511, 0, 2352, 13994400ull, "57eff24181c0581e969a40ec4972336da0891dcd093f814d7dd577c64e0142a6"},
            {3, 45000, 4, 2352, 1185760800ull, "189f89cb695ff81b40878463f326ead759437794d733329ebf3943196ad59b45"},
        }},
    };
    return identity;
}

namespace {

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> out;
    size_t i = 0;
    while (i < line.size()) {
        while (i < line.size() && std::isspace((unsigned char)line[i]))
            i++;
        if (i >= line.size())
            break;
        std::string token;
        if (line[i] == '"') {
            i++;
            while (i < line.size() && line[i] != '"')
                token += line[i++];
            i++;
        } else {
            while (i < line.size() && !std::isspace((unsigned char)line[i]))
                token += line[i++];
        }
        out.push_back(token);
    }
    return out;
}

bool to_int(const std::string& s, int& out) {
    if (s.empty())
        return false;
    for (char c : s)
        if (!std::isdigit((unsigned char)c))
            return false;
    out = std::stoi(s);
    return true;
}

bool safe_name(const std::string& name) {
    return !name.empty() && name.find('/') == std::string::npos && name.find('\\') == std::string::npos
        && name.find(':') == std::string::npos && name != "." && name != "..";
}

} /* namespace */

bool parse_gdi(const std::string& text, std::vector<GdiTrack>& out, std::string& error) {
    out.clear();
    std::istringstream in(text);
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        if (line.find_first_not_of(" \t") != std::string::npos)
            lines.push_back(line);
    }
    int count = 0;
    if (lines.empty() || !to_int(tokenize(lines[0]).empty() ? "" : tokenize(lines[0])[0], count)) {
        error = "The .gdi file does not start with a track count.";
        return false;
    }
    if (count < 1 || int(lines.size()) != count + 1) {
        error = "The .gdi track count does not match its track lines.";
        return false;
    }
    for (int i = 1; i <= count; i++) {
        const std::vector<std::string> f = tokenize(lines[i]);
        GdiTrack t;
        if (f.size() != 6 || !to_int(f[0], t.number) || !to_int(f[1], t.lba) || !to_int(f[2], t.type)
                || !to_int(f[3], t.sector_size) || !to_int(f[5], t.offset)) {
            error = "Track line " + std::to_string(i) + " in the .gdi file is malformed.";
            return false;
        }
        if (!safe_name(f[4])) {
            error = "Track file names must be in the same folder as the .gdi file.";
            return false;
        }
        t.filename = f[4];
        out.push_back(t);
    }
    return true;
}

std::string sha256_file(const std::string& path, const HashProgress& progress, uint64_t base, uint64_t total) {
    std::FILE *f = std::fopen(path.c_str(), "rb");
    if (f == nullptr)
        return "";
    CC_SHA256_CTX ctx;
    CC_SHA256_Init(&ctx);
    std::vector<unsigned char> buffer(4 << 20);
    uint64_t done = 0;
    size_t n;
    while ((n = std::fread(buffer.data(), 1, buffer.size(), f)) > 0) {
        CC_SHA256_Update(&ctx, buffer.data(), CC_LONG(n));
        done += n;
        if (progress)
            progress(base + done, total);
    }
    const bool failed = std::ferror(f) != 0;
    std::fclose(f);
    if (failed)
        return "";
    unsigned char digest[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256_Final(digest, &ctx);
    char hex[CC_SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; i++)
        std::snprintf(hex + i * 2, 3, "%02x", digest[i]);
    return hex;
}

GameCheck verify_game(const std::string& folder_or_gdi, const GameIdentity& identity, const HashProgress& progress) {
    GameCheck check;
    std::error_code ec;
    fs::path gdi;
    if (fs::is_directory(folder_or_gdi, ec)) {
        int found = 0;
        for (const auto& entry : fs::directory_iterator(folder_or_gdi, ec)) {
            std::string ext = entry.path().extension().string();
            for (char& c : ext)
                c = char(std::tolower((unsigned char)c));
            if (ext == ".gdi" && entry.is_regular_file()) {
                gdi = entry.path();
                found++;
            }
        }
        if (found != 1) {
            check.error = found == 0 ? "No .gdi file was found in the selected folder."
                                     : "The selected folder holds more than one .gdi file.";
            return check;
        }
    } else {
        gdi = folder_or_gdi;
    }
    check.gdi_path = gdi.string();
    std::ifstream in(gdi, std::ios::binary);
    if (!in || fs::file_size(gdi, ec) > 65536) {
        check.error = "The .gdi file could not be read.";
        return check;
    }
    std::stringstream text;
    text << in.rdbuf();
    std::vector<GdiTrack> tracks;
    if (!parse_gdi(text.str(), tracks, check.error))
        return check;
    if (tracks.size() != identity.tracks.size()) {
        check.error = "This is not the supported disc: it should have three tracks.";
        return check;
    }
    uint64_t total = 0;
    for (const TrackIdentity& t : identity.tracks)
        total += t.bytes;
    uint64_t base = 0;
    for (size_t i = 0; i < tracks.size(); i++) {
        const GdiTrack& t = tracks[i];
        const TrackIdentity& want = identity.tracks[i];
        if (t.number != want.number || t.lba != want.lba || t.type != want.type
                || t.sector_size != want.sector_size || t.offset != 0) {
            check.error = "Track " + std::to_string(i + 1) + " layout does not match " + identity.release + ".";
            return check;
        }
        const fs::path track = gdi.parent_path() / t.filename;
        if (!fs::is_regular_file(track, ec)) {
            check.error = "Track file " + t.filename + " is missing next to the .gdi file.";
            return check;
        }
        if (fs::file_size(track, ec) != want.bytes) {
            check.error = "Track " + std::to_string(i + 1) + " has the wrong size for " + identity.release + ".";
            return check;
        }
        const std::string sha = sha256_file(track.string(), progress, base, total);
        if (sha != want.sha256) {
            check.error = "Track " + std::to_string(i + 1) + " does not match " + identity.release + ".";
            return check;
        }
        base += want.bytes;
        check.track_paths.push_back(track.string());
    }
    check.gdi_sha256 = sha256_file(check.gdi_path);
    check.ok = true;
    return check;
}

} /* namespace ringpad */
