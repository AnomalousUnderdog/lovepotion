#include "objects/font/fontc.h"

#include "utf8/utf8.h"

using namespace love::common;

love::Type Font::type("Font", &Object::type);

int Font::fontCount = 0;

Font::Font()
{
    fontCount++;
}

Font::~Font()
{
    --fontCount;
}

float Font::GetLineHeight() const
{
    return this->lineHeight;
}

int Font::GetWidth(const std::string& string)
{
    if (string.size() == 0)
        return 0;

    int maxWidth = 0;

    try
    {
        utf8::iterator<std::string::const_iterator> i(string.begin(), string.begin(), string.end());
        utf8::iterator<std::string::const_iterator> end(string.end(), string.begin(), string.end());

        while (i != end)
        {
            uint32_t prevGlyph = 0;
            int width          = 0;

            for (; i != end && *i != '\n'; ++i)
            {
                uint32_t current = *i;

                if (current == '\r')
                    continue;

                width += this->GetWidth(prevGlyph, current);
                prevGlyph = current;
            }

            maxWidth = std::max(maxWidth, width);

            if (i != end)
                ++i;
        }
    }
    catch (utf8::exception& e)
    {
        throw love::Exception("UTF-8 decoding error: %s", e.what());
    }

    return maxWidth;
}

bool Font::GetConstant(const char* in, AlignMode& out)
{
    return alignModes.Find(in, out);
}

bool Font::GetConstant(AlignMode in, const char*& out)
{
    return alignModes.Find(in, out);
}

std::vector<const char*> Font::GetConstants(AlignMode)
{
    return alignModes.GetNames();
}

bool Font::GetConstant(const char* in, Font::SystemFontType& out)
{
    return sharedFonts.Find(in, out);
}

bool Font::GetConstant(Font::SystemFontType in, const char*& out)
{
    return sharedFonts.Find(in, out);
}

std::vector<const char*> Font::GetConstants(Font::SystemFontType)
{
    return sharedFonts.GetNames();
}

// clang-format off
constexpr StringMap<Font::AlignMode, Font::ALIGN_MAX_ENUM>::Entry alignModeEntries[] =
{
    { "left",    Font::AlignMode::ALIGN_LEFT    },
    { "right",   Font::AlignMode::ALIGN_RIGHT   },
    { "center",  Font::AlignMode::ALIGN_CENTER  },
    { "justify", Font::AlignMode::ALIGN_JUSTIFY }
};

const StringMap<Font::AlignMode, Font::ALIGN_MAX_ENUM> Font::alignModes(alignModeEntries);
// clang-format on
