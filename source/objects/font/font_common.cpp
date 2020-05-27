#include "common/runtime.h"
#include "objects/font/font.h"

using namespace love;

namespace
{
    struct Word
    {
        std::string_view text;
        float width;
        u32 line;
    };

    std::vector<Word> words;
}

void Font::Print(const std::vector<Font::ColoredString> & strings, const DrawArgs & args, float * limit, const Color & blend, Font::AlignMode align)
{
    float currentSize = 0.0f;
    auto offset = std::make_pair(0.0f, 0.0f);
    std::string line;

    for (size_t i = 0; i < strings.size(); i++)
    {
        const std::string & str = strings[i].string;
        auto currentChar = str.c_str();
        const auto end = currentChar + str.size();
        const Color & clr = strings[i].color;

        Word word;

        while (currentChar != end)
        {
            uint32_t codepoint;
            const auto bytes = decode_utf8(&codepoint, (uint8_t *)currentChar);

            if (codepoint == '\n' || (limit != nullptr && currentSize >= *limit))
            {
                auto size = this->GenerateVertices(line, offset, args, blend, clr);

                offset.first = 0.0f;
                offset.second += size.second;
            }

            if (codepoint == '\n')
                line.clear();
            else
            {
                if (limit != nullptr && currentSize >= *limit)
                {
                    currentSize = 0.0f;
                    line.clear();
                }

                line.append(currentChar, bytes);
            }

            // if (codepoint != ' ')
            //     currentSize += this->_GetGlyphWidth(codepoint);

            // if (codepoint != ' ' && bytes != -1)
            //     word.text += std::string(currentChar, bytes);
            // else
            // {
            //     word.width = currentSize;
            //     currentSize = 0.0f;
            //     words.push_back(word);
            //     word.text.clear();
            // }

            currentChar += bytes;
        }

        // offset.first += this->_GetGlyphWidth(' ');

        if (!line.empty())
        {
            auto size = this->GenerateVertices(line, offset, args, blend, clr);
            offset.first += size.first;

            line.clear();
        }
        // for (auto word : words)
        // {
        //     auto size = this->GenerateVertices(line, offset, args, blend, clr);
        //     offset.first += size.first;
        // }
    }
}

bool Font::GetConstant(const char * in, AlignMode & out)
{
    return alignModes.Find(in, out);
}

bool Font::GetConstant(AlignMode in, const char *& out)
{
    return alignModes.Find(in, out);
}

std::vector<std::string> Font::GetConstants(AlignMode)
{
    return alignModes.GetNames();
}

bool Font::GetConstant(const char * in, Font::SystemFontType & out)
{
    return sharedFonts.Find(in, out);
}

bool Font::GetConstant(Font::SystemFontType in, const char *& out)
{
    return sharedFonts.Find(in, out);
}

std::vector<std::string> Font::GetConstants(Font::SystemFontType)
{
    return sharedFonts.GetNames();
}

StringMap<Font::AlignMode, Font::ALIGN_MAX_ENUM>::Entry Font::alignModeEntries[] =
{
    { "left",    ALIGN_LEFT    },
    { "right",   ALIGN_RIGHT   },
    { "center",  ALIGN_CENTER  },
    { "justify", ALIGN_JUSTIFY }
};

StringMap<Font::AlignMode, Font::ALIGN_MAX_ENUM> Font::alignModes(Font::alignModeEntries, sizeof(Font::alignModeEntries));
