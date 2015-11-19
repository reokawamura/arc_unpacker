#include "fmt/leaf/pak2_group/pak2_audio_decoder.h"
#include "log.h"

using namespace au;
using namespace au::fmt::leaf;

static const bstr magic = "\x03\x95\xAD\x4B"_b;

bool Pak2AudioDecoder::is_recognized_impl(File &file) const
{
    file.io.seek(4);
    return file.io.read(4) == magic;
}

std::unique_ptr<File> Pak2AudioDecoder::decode_impl(File &file) const
{
    file.io.seek(12);
    const auto size_comp = file.io.read_u32_le();
    file.io.skip(4);
    const auto data = file.io.read(size_comp);
    if (!file.io.eof())
        Log.warn("Extra data after EOF.\n");
    auto output_file = std::make_unique<File>(file.name, data);
    output_file->guess_extension();
    return output_file;
}

static auto dummy = fmt::register_fmt<Pak2AudioDecoder>("leaf/pak2-audio");