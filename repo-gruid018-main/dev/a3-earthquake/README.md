# CSci-4611 Assignment 3:  Earthquake
Pierce Gruidl
gruid018

Quake Design Decisions:
For the scale and color of the quakes I use the same modifier which is just taking the range of all of the
quakes from the database and putting them in a scale from 0-1. This is then used as the R input for the RGB color
of the quakes with an already set G value of 0.22 so that as the magnitude of the quakes increases the color of
the spheres that represent them gets closer to red and the more mild quakes retain more of the green base color.
the same scale is used when changing the size of the quakes that range from the smallest values size to 0.12 the
scale of the mingfx sphere.