#pragma once
#include "vars.hpp"
#include "math/Vector.hpp"
#include "math/VMatrix.hpp"

#pragma warning(disable: 26495)

class Player_t;

struct ray_t
{
	Vector m_start;
	PAD(4);
	Vector m_delta;
	PAD(40);
	bool m_isRay = true;
	bool m_isSwept;

	void initialize(const Vector& src, const Vector& dest)
	{
		m_start = src;
		m_delta = dest - src;
		m_isSwept = m_delta.x || m_delta.y || m_delta.z;
	}

	ray_t(const Vector& src, const Vector& dest) :
		m_start(src), m_delta(dest - src)
	{
		m_isSwept = m_delta.x || m_delta.y || m_delta.z;
	}
	ray_t() = default;
};

struct csurface_t
{
	const char* m_name;
	short m_props;
	unsigned short m_flags;
};

struct cplane_t
{
	Vector m_normal;
	float m_distance;
	char m_type;
	char m_signBits;
	PAD(2);
};

struct trace_t
{
	Vector m_start;
	Vector m_end;
	cplane_t m_plane;
	float m_fraction;
	int m_contents;
	unsigned short m_flags;
	bool m_allSolid;
	bool m_startSolid;
	float m_fractionLeftSolid;
	csurface_t m_surface;
	int m_hitgroup;
	short m_physicsBone;
	Player_t* m_entity;
	int m_hitbox;
};

struct traceFilter
{
public:
	traceFilter(Player_t* entity) :
		m_skip{ entity }
	{}
	traceFilter() = default;
	virtual bool shouldHitEntity(Player_t* ent, int) const
	{
		return ent != m_skip;
	}
	virtual int getTraceType() const
	{
		return 0;
	}
	void* m_skip;
};


class IEngineTrace
{
public:
	VFUNC(int, getPointContents, 0, (const Vector& absPosition, int contentsMask), (this, std::cref(absPosition), contentsMask, nullptr));
	VFUNC(void, traceRay, 5, (const ray_t& ray, unsigned int mask, const traceFilter* filter, trace_t* trace), (this, std::cref(ray), mask, filter, trace));
};