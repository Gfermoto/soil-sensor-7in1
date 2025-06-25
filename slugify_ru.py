#!/usr/bin/env python3
"""Custom slugify function for MkDocs that converts Cyrillic headings to latin slugs.

Placed at project root so it can be imported as ``slugify_ru``.
"""
import re
from slugify import slugify as _slugify

# Pattern to strip emoji characters and other pictograms that confuse slugs
_EMOJI_RE = re.compile(r"[\u2600-\u27BF\U0001F300-\U0001FAD6]+", re.UNICODE)

def custom(value: str, separator: str = '-') -> str:
    """Return URL-friendly slug.

    Steps:
    1. Remove emoji/pictograms.
    2. Use python-slugify to transliterate Cyrillic to Latin.
    3. Force lowercase, limit length, keep ASCII only.
    """
    value = _EMOJI_RE.sub('', value)
    return _slugify(value, separator=separator, lowercase=True, locale='ru', max_length=60) 