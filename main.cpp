#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include "sphere.h"
#include "hitablelist.h"
#include "float.h"
#include "camera.h"
#include "material.h"
#include "drand48.h"
#include "lights.h"
#include "rectangle.h"
#include "box.h"
#include "translate.h"
#include "rotate.h"
//#include "volume.h"
#include "bvh.h"
#include "pdf.h"
#include <thread>

inline vec3 de_nan(const vec3& c)
{
    vec3 temp = c;
    if(!(temp[0] == temp[0])) temp[0] = 0;
    if(!(temp[1] == temp[1])) temp[1] = 0;
    if(!(temp[2] == temp[2])) temp[2] = 0;
    return temp;
}

// hitable *random_scene()
// {
//     int n = 50000;
//     hitable **list = new hitable*[n+1];
//     texture *checker = new checker_texture(new contant_texture(vec3(0.2, 0.3, 0.1)), new contant_texture(vec3(0.9, 0.9, 0.9)));
//     list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(checker));
//     int i = 1;
//     for(int a = -10; a < 10; a++)
//     {
//         for(int b = -10; b < 10; b++)
//         {
//             float choose_mat = drand48();
//             vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
//             if((center - vec3(4, 0.2, 0)).length() > 0.9)
//             {
//                 if(choose_mat < 0.8)
//                 {
//                     //list[i++] = new sphere(center, 0.2, new lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
//                     list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2, new lambertian(new contant_texture(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48()))));
//                 }
//                 else if(choose_mat < 0.95)
//                 {
//                     list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
//                 }
//                 else
//                 {
//                     list[i++] = new sphere(center, 0.2, new dielectric(1.5));
//                 }
//             }
//         }
//     }
//     list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
//     list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new contant_texture(vec3(0.4, 0.2, 0.1))));
//     list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

//     return new hitable_list(list, i);
// }

// hitable *two_spheres()
// {
//     texture *checker = new checker_texture(new contant_texture(vec3(0.2, 0.3, 0.1)), new contant_texture(vec3(0.9, 0.9, 0.9)));
//     int n = 50;
//     hitable **list = new hitable*[n + 1];
//     list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
//     list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));

//     return new hitable_list(list, 2);
// }

// hitable *two_perlin_spheres()
// {
//     texture *pertext = new noise_texture(2);
//     hitable **list = new hitable*[2];
//     list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
//     list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
//     return new hitable_list(list, 2);
// }

// hitable *earth()
// {
//     int nx, ny, nn;
//     unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
//     material *mat = new lambertian(new image_texture(tex_data, nx, ny));
//     return new sphere(vec3(0, 0, 0), 2, mat);
// }

// hitable *simple_light()
// {
//     texture *pertext = new noise_texture(4);
//     hitable **list = new hitable*[4];
//     list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
//     list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
//     list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new contant_texture(vec3(4, 4, 4))));
//     list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new contant_texture(vec3(4, 4, 4))));
//     return new hitable_list(list, 4);
// }

// hitable *cornell_box()
// {
//     hitable **list = new hitable*[8];
//     int i = 0;
//     material *red = new lambertian(new contant_texture(vec3(0.65, 0.05, 0.05)));
//     material *white = new lambertian(new contant_texture(vec3(0.73, 0.73, 0.73)));
//     material *green = new lambertian(new contant_texture(vec3(0.12, 0.45, 0.15)));
//     material *light = new diffuse_light(new contant_texture(vec3(15, 15, 15)));
//     list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
//     list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
//     list[i++] = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
//     list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
//     list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
//     list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
//     list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
//     list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
//     return new hitable_list(list, i);
// }

void cornell_box(hitable **scene, camera **cam, float aspect)
{
    hitable **list = new hitable*[8];
    int i = 0;
    material *red = new lambertian(new contant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new contant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new contant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new contant_texture(vec3(15, 15, 15)));
    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new flip_normals(new xz_rect(213, 343, 227, 332, 554, light));
    list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
    material *glass = new dielectric(1.5);
    list[i++] = new sphere(vec3(190, 90, 190), 90, glass);
    //list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
    material *aluminum = new metal(vec3(0.8, 0.85, 0.88), 0.0);
    //material *aluminum = new lambertian(new contant_texture(vec3(0.5, 0.8, 0.6)));
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), aluminum), 15), vec3(265, 0, 295));
    *scene = new hitable_list(list, i);
    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278, 278, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    *cam = new camera(lookfrom, lookat, vec3(0, 1, 0), vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);
}

// hitable *cornell_smoke()
// {
//     hitable **list = new hitable*[8];
//     int i = 0;
//     material *red = new lambertian(new contant_texture(vec3(0.65, 0.05, 0.05)));
//     material *white = new lambertian(new contant_texture(vec3(0.73, 0.73, 0.73)));
//     material *green = new lambertian(new contant_texture(vec3(0.12, 0.45, 0.15)));
//     material *light = new diffuse_light(new contant_texture(vec3(7, 7, 7)));
//     list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
//     list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
//     list[i++] = new xz_rect(113, 443, 127, 432, 554, light);
//     list[i++] = new flip_normals(new xz_rect(0, 555, 0, 555, 555, white));
//     list[i++] = new xz_rect(0, 555, 0, 555, 0, white);
//     list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
//     hitable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
//     hitable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
//     list[i++] = new constant_medium(b1, 0.01, new contant_texture(vec3(1.0, 1.0, 1.0)));
//     list[i++] = new constant_medium(b2, 0.01, new contant_texture(vec3(0.0, 0.0, 0.0)));
//     return new hitable_list(list, i);
// }

// hitable *final()
// {
//     int nb = 20;
//     hitable **list = new hitable*[30];
//     hitable **boxlist = new hitable*[10000];
//     hitable **boxlist2 = new hitable*[10000];
//     material *white = new lambertian(new  contant_texture(vec3(0.73, 0.73, 0.73)));
//     material *ground = new lambertian(new contant_texture(vec3(0.48, 0.83, 0.530)));
//     int b = 0;
//     for(int i = 0; i < nb; i++)
//     {
//         for(int j = 0; j < nb; j++)
//         {
//             float w = 100;
//             float x0 = -1000 + i * w;
//             float z0 = -1000 + j * w;
//             float y0 = 0;
//             float x1 = x0 + w;
//             float y1 = 100 * (drand48() + 0.01);
//             float z1 = z0 + w;
//             boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
//         }
//     }
//     int l = 0;
//     list[l++] = new bvh_node(boxlist, b, 0, 1);
//     material *light = new diffuse_light(new contant_texture(vec3(7, 7, 7)));
//     list[l++] = new xz_rect(123, 423, 147, 412, 554, light);
//     vec3 center(400, 400, 200);
//     list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new contant_texture(vec3(0.7, 0.3, 0.1))));
//     list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
//     list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));
//     hitable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
//     list[l++] = boundary;
//     list[l++] = new constant_medium(boundary, 0.2, new contant_texture(vec3(0.2, 0.4, 0.9)));
//     boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
//     list[l++] = new constant_medium(boundary, 0.0001, new contant_texture(vec3(1.0, 1.0, 1.0)));
//     int nx, ny, nn;
//     unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
//     material *emat = new lambertian(new image_texture(tex_data, nx, ny));
//     list[l++] = new sphere(vec3(400, 200, 400), 100, emat);
//     texture *pertext = new noise_texture(0.1);
//     list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));
//     int ns = 1000;
//     for(int k = 0; k < ns; k++)
//     {
//         boxlist2[k] = new sphere(vec3(165 * drand48(), 165 * drand48(), 165 * drand48()), 10, white);
//     }
//     list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));
//     return new hitable_list(list, l);
// }

vec3 color(const ray& r, hitable *world, hitable *light_shape, int depth)
{
    hit_recoord hrec;
    if(world->hit(r, 0.001, FLT_MAX, hrec))
    {
        scatter_record srec;
        vec3 emitted = hrec.mat_ptr->emitted(r, hrec, hrec.u, hrec.v, hrec.p);
        if(depth < 50 && hrec.mat_ptr->scatter(r, hrec, srec))
        {
            if(srec.is_specular)
            {
                return srec.attenuation * color(srec.specular_ray, world, light_shape, depth + 1);
            }
            else
            {
                hitable_pdf plight(light_shape, hrec.p);
                mixture_pdf p(&plight, srec.pdf_ptr);
                ray scattered = ray(hrec.p, p.generate(), r.time());
                float pdf_val = p.value(scattered.direction());
                delete srec.pdf_ptr;
                return emitted + srec.attenuation * hrec.mat_ptr->scattering_pdf(r, hrec, scattered) * color(scattered, world, light_shape, depth + 1) / pdf_val;
            }
        }
        else
        {
            return emitted;
        }
    }
    else
    {
        // vec3 unit_direction = unit_vector(r.direction());
        // float t = 0.5 * (unit_direction.y() + 1.0);
        // return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        return vec3(0, 0, 0);
    }
}


int main() {

    int nx = 500;
    int ny = 500;
    int ns = 10000;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";

    //float R = cos(M_PI / 4);
    // hitable *list[5];
    // list[0] = new sphere(vec3(0, 0, -1.0), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    // list[1] = new sphere(vec3(0, -100.5, -1.0), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    // list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    // list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    // list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    // hitable *world = new hitable_list(list, 5);
    // hitable *world = cornell_box();

    // vec3 lookfrom(278, 278, -800);
    // vec3 lookat(278, 278, 0);
    // float dist_to_focus = 10;
    // float aperture = 0.0;
    // float vfov = 40.0;

    // camera cam(lookfrom, lookat, vec3(0, 1, 0), vfov, (float(nx) / float(ny)), aperture, dist_to_focus, 0.0, 1.0);
    hitable *world;
    camera *cam;
    cornell_box(&world, &cam, (float(ny) / float(nx)));
    hitable *light_shape = new xz_rect(213, 343, 227, 332, 554, 0);
    //hitable *glass_sphere = new sphere(vec3(190, 90, 190), 90, 0);
    hitable *a[1];
    a[0] = light_shape;
    //a[1] = glass_sphere;
    hitable_list hlist(a, 1);
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            vec3 col(0, 0, 0);
            for(int s = 0; s < ns; s++)
            {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam->get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += de_nan(color(r, world, &hlist, 0));
            }
            // float u = float(i) / float(nx);
            // float v = float(j) / float(ny);
            // ray r = cam.get_ray(u, v);
            // vec3 p = r.point_at_parameter(2.0);
            // col += color(r, world, 0);
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }

    return 0;
}