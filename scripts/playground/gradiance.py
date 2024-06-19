import mitsuba as mi
import drjit as dr

import numpy as np
import matplotlib.pyplot as plt

# initialize variant
mi.set_variant("cuda_ad_rgb")

coh = dr.zeros(mi.Coherence3f, 5)
print(coh)

gr = dr.zeros(mi.GeneralizedRadiance3f, 5)
gr.L += dr.arange(dr.scalar.ArrayXf,    5)
print(gr, dr.arange(dr.scalar.ArrayXf,  5))