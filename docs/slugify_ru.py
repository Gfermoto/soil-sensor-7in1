#!/usr/bin/env python3
"""Custom slugify for MkDocs that converts Cyrillic headings to latin ASCII slugs.
Placed in docs/ so import path is ``docs.slugify_ru.custom``.
"""
import re
from slugify import slugify as _slugify

_EMOJI_RE = re.compile(r"[\u2600-\u27BF\U0001F300-\U0001FAD6]+", re.UNICODE)

def custom(value: str, separator: str = '-') -> str:
    """Return URL-friendly slug with transliteration.
    1. Remove emoji.
    2. Transliterate Cyrillic to Latin.
    3. Lower-case & trim to 60 chars.
    """
    value = _EMOJI_RE.sub('', value)
    return _slugify(value, separator=separator, lowercase=True, locale='ru', max_length=60) 