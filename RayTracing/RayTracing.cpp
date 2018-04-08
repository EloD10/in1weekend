// RayTracing.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <float.h>
#include <time.h>
#include <random>
#include "vec3.h"
#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
using namespace std;

//vec3 color(const ray& r)
//{
//	vec3 unit_direction = unit_vector(r.direction());
//	float t = 0.5 * (unit_direction.y() + 1.0);	// y取值在[-1, 1]之间，这里映射到[0, 1]之间
//	return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
//}

// Chapter 4
//bool hit_sphere(const vec3& center, float radius, const ray& r)
//{
//	vec3 oc = r.origin() - center;
//	float a = dot(r.direction(), r.direction());
//	float b = 2.0 * dot(oc, oc);
//	float c = dot(center, center) - radius * radius;
//	float discriminant = b * b - 4 * a * c;
//	return discriminant > 0;
//}

// Chapter 4
//vec3 color(const ray& r)
//{
//	if (hit_sphere(vec3(0, 0, -1), 0.5, r))
//	{
//		return vec3(1, 0, 0);
//	}
//	vec3 unit_direction = unit_vector(r.direction());
//	float t = 0.5 * (unit_direction.y() + 1.0);	// y取值在[-1, 1]之间，这里映射到[0, 1]之间
//	return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
//}

float hit_sphere(const vec3& center, float radius, const ray& r)
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
	{
		return -1.0;
	}
	else
	{
		return (-b - sqrt(discriminant)) / (2.0 * a);
	}
}

// Chapter 5
//vec3 color(const ray& r)
//{
//	vec3 sphereCenter = vec3(0, 0, -1);
//	float t = hit_sphere(sphereCenter, 0.5, r);
//	if (t > 0.0)
//	{
//		vec3 N = unit_vector(r.point_at_parameter(t) - sphereCenter);
//		return vec3(N.x() + 1.0, N.y() + 1.0, N.z() + 1.0) * 0.5;
//	}
//	vec3 unit_direction = unit_vector(r.direction());
//	t = 0.5 * (unit_direction.y() + 1.0);	// y取值在[-1, 1]之间，这里映射到[0, 1]之间
//	return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
//}

// Chapter 6
//vec3 color(const ray& r, hitable* world)
//{
//	hit_record rec;
//	if (world->hit(r, 0.0, FLT_MAX, rec))
//	{
//		return vec3(rec.normal.x() + 1.0, rec.normal.y() + 1.0, rec.normal.z() + 1.0) * 0.5;
//	}
//	else
//	{
//		vec3 unit_direction = unit_vector(r.direction());
//		float t = 0.5 * (unit_direction.y() + 1.0);	// y取值在[-1, 1]之间，这里映射到[0, 1]之间
//		return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
//	}
//}

//float drand48()
//{
//	int i = rand() % 100;
//	return float(i) / 100.0;
//}
//
//vec3 random_in_unit_sphere()
//{
//	vec3 p;
//	do
//	{
//		p = vec3(drand48(), drand48(), drand48()) * 2.0 - vec3(1, 1, 1);
//	} while (dot(p, p) >= 1.0);
//	return p;
//}

// Chapter 7
//vec3 color(const ray& r, hitable* world)
//{
//	hit_record rec;
//	if (world->hit(r, 0.0, FLT_MAX, rec))
//	{
//		vec3 target = rec.p + rec.normal + random_in_unit_sphere();
//		return color(ray(rec.p, target - rec.p), world) * 0.5;	// 漫反射ray
//	}
//	else
//	{
//		vec3 unit_direction = unit_vector(r.direction());
//		float t = 0.5 * (unit_direction.y() + 1.0);	// y取值在[-1, 1]之间，这里映射到[0, 1]之间
//		return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
//	}
//}

// Chapter 8
vec3 color(const ray& r, hitable* world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return color(scattered, world, depth + 1) * attenuation;
		}
		else
		{
			return vec3(0, 0, 0);
		}
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5 * (unit_direction.y() + 1.0);	// y取值在[-1, 1]之间，这里映射到[0, 1]之间
		return vec3(1.0f, 1.0f, 1.0f) * (1.0f - t) + vec3(0.5f, 0.7f, 1.0f) * t;
	}
}

int main()
{
	ofstream file("1.ppm");
	if (file.is_open())
	{
		int width = 200;
		int height = 100;
		vec3 origin(0.0f, 0.0f, 0.0f);
		vec3 lower_left_corner(-2.0f, -1.0f, -1.0f);
		vec3 horizontal(4.0f, 0.0f, 0.0f);
		vec3 vertical(0.0f, 2.0f, 0.0f);
		hitable* list[4];
		list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
		list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0)));
		list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
		list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
		hitable* world = new hitable_list(list, 4);
		camera cam;
		srand(time(0));
		file << "P3\n" << width << " " << height << "\n255\n";
		for (int j = height - 1; j >= 0; j--)
		{
			for (int i = 0; i < width; i++)
			{
				//float u = float(i) / float(width);
				//float v = float(j) / float(height);
				//ray r(origin, lower_left_corner + horizontal * u + vertical * v);
				//vec3 col = color(r);
				vec3 col = vec3(0, 0, 0);
				for (int k = 0; k < 100; k++)
				{
					float u = float(i + drand48()) / float(width);
					float v = float(j + drand48()) / float(height);
					ray r = cam.get_ray(u, v);
					col = col + color(r, world, 0);
				}
				col = col / 100.0;
				col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
				int ir = int(255.9f * col.r());
				int ig = int(255.9f * col.g());
				int ib = int(255.9f * col.b());
				file << ir << " " << ig << " " << ib << "\n";
			}
		}
		file.close();
	}
	return 0;
}
