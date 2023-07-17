from uwimg import *
im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
lfreq = convolve_image(im, f, 1)
hfreq = im - lfreq
reconstruct = lfreq + hfreq
save_image(lfreq, "low-frequency")
save_image(hfreq, "high-frequency")
save_image(reconstruct, "reconstruct")


ron = load_image("data/ron.png")

f = make_gaussian_filter(3)
lfreq_ron = convolve_image(ron, f, 1)
hfreq_ron = ron - lfreq_ron
reconstruct_ron = lfreq_ron + hfreq_ron
save_image(lfreq_ron, "low-frequency_ron")
save_image(hfreq_ron, "high-frequency_ron")
save_image(reconstruct_ron, "reconstruct_ron")

dumbledore = load_image("data/dumbledore.png")
lfreq_dumbledore = convolve_image(dumbledore, f, 1)
hfreq_dumbledore = dumbledore - lfreq_dumbledore
reconstruct_dumbledore = lfreq_dumbledore + hfreq_dumbledore
save_image(lfreq_dumbledore, "low-frequency_dumbledore")
save_image(hfreq_dumbledore, "high-frequency_dumbledore")
save_image(reconstruct_dumbledore, "reconstruct_dumbledore")

feature_normalize(hfreq_ron)
feature_normalize(lfreq_dumbledore)
save_image(hfreq_ron, "high-frequency_ron_fn")
save_image(lfreq_dumbledore, "low-frequency_dumbledore_fn")


reconstruct_lh = lfreq_dumbledore+ hfreq_ron
feature_normalize(reconstruct_lh)
save_image(reconstruct_lh, "ronbledore")

save_image(bilinear_resize(reconstruct_lh,42,60), "ronbledore_small_xx")
save_image(bilinear_resize(reconstruct_lh,126,180), "ronbledore_small")
save_image(reconstruct_lh, "ronbledore")