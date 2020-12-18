#ifndef _TEK_SKELETON_HPP
#define _TEK_SKELETON_HPP

#include "../core/tek_core.hpp"
#include "../math/tek_math.hpp"


typedef struct
{
	Vec3 position;
	Quat rotation;
	Vec3 size;
	char name[64];
	int parent_id;
	int children[4];
	u32 num_children;
} TekBone;

typedef struct
{
	char name[64];
	TekBone *bones;
	u32 num_bones;
} TekAnimation;


void tek_animation_destroy(TekAnimation *anim);

bool tek_animation_load(TekAnimation *anim, const char *filename);

typedef struct
{
	TekAnimation *animations;
	u32 num_animations;
	u32 cur_animation;

	TekBone *bones;
	u32 num_bones;
	int root_id;
	Mat4 *matrices;
} TekSkeleton;

void tek_skeleton_destroy(TekSkeleton *skelet);

bool tek_skeleton_add_animation(TekSkeleton *skelet, TekAnimation *anim);

void tek_skeleton_play_animation(TekSkeleton *skelet, u32 index);

void tek_skeleton_calc_pose(TekSkeleton *skelet, float frame);

void tek_skeleton_update(TekSkeleton *skelet, float delta);

bool tek_skeleton_load(TekSkeleton *skelet, const char *filename);

#endif