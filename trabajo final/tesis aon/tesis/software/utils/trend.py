# -------- Module imports ------- #
from dataclasses import dataclass

@dataclass
class TrendData:
    avg: int
    max: int
    min: int

@dataclass
class Trend:
    rms:      TrendData
    twd:      TrendData = 0
    harmonic: list[TrendData] = 0