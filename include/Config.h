//
// Created by i.kalinin on 04.07.2019.
//

#ifndef LIBSITL_CONFIG_H
#define LIBSITL_CONFIG_H

#if __GNUC__ >= 4
#define SITL_API __attribute__ ((__visibility__ ("default")))
#elif
#define SITL_API
#endif

#endif //LIBSITL_CONFIG_H
