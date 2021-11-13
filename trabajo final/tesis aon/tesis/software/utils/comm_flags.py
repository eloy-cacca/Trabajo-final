# -------- Module imports ------- #
from dataclasses import dataclass


@dataclass
class CommFlags:
    TRENDS_FREQ_CHANGED:                  int = 1 << 0
    TRENDS_AMP_CHANGED:                   int = 1 << 1
    TRENDS_ENABLED_CHANGED:               int = 1 << 2
    TRENDS_CUSTOM_WAVEFORM_CHANGED:       int = 1 << 3

    @staticmethod
    def check(flags: int):
        """
        Checks if all flag bits are cleared, else returns the flag
        that's high.
        """
        flags_up = []

        if flags & CommFlags.TRENDS_FREQ_CHANGED:
            flags_up.append("TRENDS_FREQ_CHANGED")

        if (flags & CommFlags.TRENDS_AMP_CHANGED):
            flags_up.append("TRENDS_AMP_CHANGED")

        if (flags & CommFlags.TRENDS_ENABLED_CHANGED):
            flags_up.append("TRENDS_ENABLED_CHANGED")

        if (flags & CommFlags.TRENDS_CUSTOM_WAVEFORM_CHANGED):
            flags_up.append("TRENDS_CUSTOM_WAVEFORM_CHANGED")

        if not flags_up:
            return False
        else:
            return flags_up
