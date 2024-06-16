import argparse

import mitsuba as mi

def main(args):
    # get bsdf params
    inv_period = args.inv_period

if __name__ == '__main__':
    # initialize variant
    mi.set_variant("cuda_ad_rgb")

    parser = argparse.ArgumentParser(description="Analize dispersion diagram of a diffraction grating wBSDF")
    parser.add_argument("--inv-period", type=float)
    args = parser.parse_args() 
    main(args)
