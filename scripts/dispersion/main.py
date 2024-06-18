import argparse

import mitsuba as mi
import drjit as dr

import numpy as np
import matplotlib.pyplot as plt

from utils import *

def diffuse_dispersion(args):
    bsdf = mi.load_dict({
        'type': 'diffuse',
        'reflectance': {
            'type': 'rgb',
            'value': [0.3, 0.5, 0.7]
        }
    })

    mi

    # bsdf = mi.load_dict({
    #     'type': 'roughconductor',
    #     'alpha': 0.2,
    #     'distribution': 'ggx'
    # })
    # since diffuse materials are not delta materials, we eval on the whole hemisphere

    # Create a (dummy) surface interaction to use for the evaluation of the BSDF
    si = dr.zeros(mi.SurfaceInteraction3f)

    # Specify an incident direction with 0 degrees elevation (normal incidence)
    si.wi = sph_to_dir(dr.deg2rad(45.0), 0.0)

    # Create grid in spherical coordinates and map it onto the sphere
    res = 400
    theta_o = dr.linspace(mi.Float, 0, 2 * dr.pi, 2 * res)

    wo = sph_to_dir(theta_o, 0.0)

    # Evaluate the whole array (18000 directions) at once
    values = bsdf.eval(mi.BSDFContext(), si, wo)
    values_np = np.array(values)

    # Extract channels of BRDF values
    values_r = values_np[:, 0]
    values_g = values_np[:, 1]
    values_b = values_np[:, 2]

    # Plot values for spherical coordinates (upper hemisphere)
    fig, ax = plt.subplots(figsize=(8, 4), subplot_kw={'projection': 'polar'})
    ax.set_thetamin(-90)
    ax.set_thetamax(90)
    ax.set_theta_offset(np.pi / 2.0)

    # plot red, green, blue separately
    ax.plot(np.array(theta_o), values_r, '-r')
    ax.plot(np.array(theta_o), values_g, '-g')
    ax.plot(np.array(theta_o), values_b, '-b')

    plt.show()

def grating_dispersion(args):
    pass

def main(args):

    # create diffuse dispersion diagram
    diffuse_dispersion(args)

    # create grating dispersion diagram
    grating_dispersion(args)

if __name__ == '__main__':
    # initialize variant
    mi.set_variant("cuda_ad_rgb")

    parser = argparse.ArgumentParser(description="Analize dispersion diagram of a diffraction grating wBSDF")
    parser.add_argument("--inv-period", type=float)
    args = parser.parse_args() 
    main(args)
