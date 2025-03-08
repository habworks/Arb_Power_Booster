/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#include <stdarg.h>
#include <touchgfx/TextProvider.hpp>
#include <touchgfx/Texts.hpp>
#include <touchgfx/TypedText.hpp>
#include <touchgfx/Unicode.hpp>
#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <texts/TypedTextDatabase.hpp>

uint16_t touchgfx::Font::getStringWidth(const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(TEXT_DIRECTION_LTR, text, pArg);
    va_end(pArg);
    return width;
}

uint16_t touchgfx::Font::getStringWidth(touchgfx::TextDirection textDirection, const touchgfx::Unicode::UnicodeChar* text, ...) const
{
    va_list pArg;
    va_start(pArg, text);
    uint16_t width = getStringWidthLTR(textDirection, text, pArg);
    va_end(pArg);
    return width;
}

touchgfx::Unicode::UnicodeChar touchgfx::TextProvider::getNextLigature(TextDirection direction)
{
    if (fontGsubTable && nextCharacters.peekChar())
    {
        substituteGlyphs();
        if (nextCharacters.peekChar(1) == 0x093F) // Hindi I-matra
        {
            nextCharacters.replaceAt1(nextCharacters.peekChar());
            nextCharacters.replaceAt0(0x093F);
        }
    }
    return getNextChar();
}

void touchgfx::TextProvider::initializeInternal()
{
    fillInputBuffer();
}

void touchgfx::LCD::drawString(touchgfx::Rect widgetArea, const touchgfx::Rect& invalidatedArea, const touchgfx::LCD::StringVisuals& stringVisuals, const touchgfx::Unicode::UnicodeChar* format, ...)
{
    va_list pArg;
    va_start(pArg, format);
    drawStringLTR(widgetArea, invalidatedArea, stringVisuals, format, pArg);
    va_end(pArg);
}

// Default TypedTextDatabase
extern const touchgfx::TypedText::TypedTextData* const typedTextDatabaseArray[];

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const touchgfx::Unicode::UnicodeChar texts_all_languages[] TEXT_LOCATION_FLASH_ATTRIBUTE = {
    0x77, 0x77, 0x77, 0x2e, 0x49, 0x4d, 0x52, 0x45, 0x6e, 0x67, 0x69, 0x6e, 0x65, 0x65, 0x72, 0x69, 0x6e, 0x67, 0x2e, 0x63, 0x6f, 0x6d, 0x0, // @0 "www.IMREngineering.com"
    0x41, 0x72, 0x62, 0x20, 0x50, 0x6f, 0x77, 0x65, 0x72, 0x20, 0x42, 0x6f, 0x6f, 0x73, 0x74, 0x65, 0x72, 0x0, // @23 "Arb Power Booster"
    0x4e, 0x65, 0x67, 0x61, 0x74, 0x69, 0x76, 0x65, 0x20, 0x53, 0x75, 0x70, 0x70, 0x6c, 0x79, 0x3a, 0x20, 0x0, // @41 "Negative Supply: "
    0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x76, 0x65, 0x20, 0x53, 0x75, 0x70, 0x70, 0x6c, 0x79, 0x3a, 0x20, 0x0, // @59 "Positive Supply: "
    0x46, 0x57, 0x20, 0x52, 0x45, 0x56, 0x3a, 0x20, 0x2, 0x0, // @77 "FW REV: <>"
    0x48, 0x57, 0x20, 0x52, 0x45, 0x56, 0x3a, 0x20, 0x2, 0x0, // @87 "HW REV: <>"
    0x44, 0x65, 0x76, 0x69, 0x63, 0x65, 0x20, 0x53, 0x74, 0x61, 0x74, 0x75, 0x73, 0x3a, 0x0, // @97 "Device Status:"
    0x49, 0x73, 0x65, 0x74, 0x3a, 0x20, 0x2, 0x41, 0x0, // @112 "Iset: <>A"
    0x52, 0x65, 0x73, 0x65, 0x74, 0x20, 0x4d, 0x69, 0x6e, 0x20, 0x4d, 0x61, 0x78, 0x0, // @121 "Reset Min Max"
    0x45, 0x6e, 0x61, 0x62, 0x6c, 0x65, 0x20, 0x4c, 0x69, 0x6d, 0x69, 0x74, 0x0, // @135 "Enable Limit"
    0x53, 0x65, 0x74, 0x20, 0x4c, 0x69, 0x6d, 0x69, 0x74, 0x73, 0x3a, 0x0, // @148 "Set Limits:"
    0x2, 0x56, 0x0, // @160 "<>V"
    0x2d, 0x30, 0x30, 0x2e, 0x30, 0x30, 0x30, 0x0, // @163 "-00.000"
    0x43, 0x6f, 0x6e, 0x66, 0x69, 0x67, 0x3a, 0x0, // @171 "Config:"
    0x2d, 0x31, 0x39, 0x2e, 0x38, 0x0, // @179 "-19.8"
    0x30, 0x2e, 0x30, 0x2e, 0x30, 0x0, // @185 "0.0.0"
    0x32, 0x30, 0x2e, 0x32, 0x0, // @191 "20.2"
    0x41, 0x6d, 0x61, 0x78, 0x0, // @196 "Amax"
    0x41, 0x6d, 0x69, 0x6e, 0x0, // @201 "Amin"
    0x41, 0x72, 0x6d, 0x73, 0x0, // @206 "Arms"
    0x2d, 0x31, 0x30, 0x0, // @211 "-10"
    0x31, 0x4d, 0x2126, 0x0, // @215 "1M?"
    0x35, 0x30, 0x2126, 0x0, // @219 "50?"
    0x43, 0x48, 0x31, 0x0, // @223 "CH1"
    0x43, 0x48, 0x32, 0x0, // @227 "CH2"
    0x53, 0x45, 0x54, 0x0, // @231 "SET"
    0x4f, 0x4b, 0x0, // @235 "OK"
    0x4f, 0x4e, 0x0, // @238 "ON"
    0x2e, 0x0 // @241 "."
};

TEXT_LOCATION_FLASH_PRAGMA
KEEP extern const uint32_t indicesGb[] TEXT_LOCATION_FLASH_ATTRIBUTE;

// Array holding dynamically installed languages
struct TranslationHeader
{
    uint32_t offset_to_texts;
    uint32_t offset_to_indices;
    uint32_t offset_to_typedtext;
};
static const TranslationHeader* languagesArray[1] = { 0 };

// Compiled and linked in languages
static const uint32_t* const staticLanguageIndices[] = {
    indicesGb
};

touchgfx::LanguageId touchgfx::Texts::currentLanguage = static_cast<touchgfx::LanguageId>(0);
static const touchgfx::Unicode::UnicodeChar* currentLanguagePtr = 0;
static const uint32_t* currentLanguageIndices = 0;

void touchgfx::Texts::setLanguage(touchgfx::LanguageId id)
{
    const touchgfx::TypedText::TypedTextData* currentLanguageTypedText = 0;
    if (id < 1)
    {
        if (languagesArray[id] != 0)
        {
            // Dynamic translation is added
            const TranslationHeader* translation = languagesArray[id];
            currentLanguagePtr = (const touchgfx::Unicode::UnicodeChar*)(((const uint8_t*)translation) + translation->offset_to_texts);
            currentLanguageIndices = (const uint32_t*)(((const uint8_t*)translation) + translation->offset_to_indices);
            currentLanguageTypedText = (const touchgfx::TypedText::TypedTextData*)(((const uint8_t*)translation) + translation->offset_to_typedtext);
        }
        else
        {
            // Compiled and linked in languages
            currentLanguagePtr = texts_all_languages;
            currentLanguageIndices = staticLanguageIndices[id];
            currentLanguageTypedText = typedTextDatabaseArray[id];
        }
    }

    if (currentLanguageTypedText)
    {
        currentLanguage = id;
        touchgfx::TypedText::registerTypedTextDatabase(currentLanguageTypedText,
                                                       TypedTextDatabase::getFonts(), TypedTextDatabase::getInstanceSize());
    }
}

void touchgfx::Texts::setTranslation(touchgfx::LanguageId id, const void* translation)
{
    languagesArray[id] = (const TranslationHeader*)translation;
}

const touchgfx::Unicode::UnicodeChar* touchgfx::Texts::getText(TypedTextId id) const
{
    return &currentLanguagePtr[currentLanguageIndices[id]];
}
