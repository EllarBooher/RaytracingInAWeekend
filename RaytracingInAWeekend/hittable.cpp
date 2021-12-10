#include "ray.h"
#include "rtweekend.h"

class material;

struct hit_record {
	point3 point;
	double3 normal;
	std::shared_ptr<material> mat_ptr;
	double t;
	bool front_face;

	inline void set_face_normal(const ray& r, const double3& outward_normal) {
		front_face = linear_algebra::dot(r.dir, outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

inline void write_hit_record(std::ostream& out, const hit_record& record)
{
	out << record.point << ", " << record.normal << ", " << record.t << ", " << record.front_face << std::endl;
}