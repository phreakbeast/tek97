#include "tek_skeleton.hpp"


#include <stdio.h>
#include <string.h>

	static void parent_to(TekBone *bone, TekBone *parent);


void tek_skeleton_destroy(TekSkeleton *skelet)
	{
		tek_free(skelet->bones);
	}

bool tek_skeleton_load(TekSkeleton *skelet, const char *filename)
	{

		FILE *fp = fopen(filename, "rb");
		if (!fp)
		{
			printf("Error: cannot open file %s\n", filename);
			return 0;
		}

		u32 bone_ctr = 0;

		char line[1024];
		while (fgets(line, 1024, fp))
		{
			if (line[0] == 'h' && line[1] == 'd' && line[2] == 'r')
			{
				u32 num_bones = 0;
				int result = sscanf(line, "hdr %u", &num_bones);
				if (result < 1)
				{
					printf("Warning: cannot process line: %s File: %s\n", line, filename);
					fclose(fp);
					return false;
				}

				skelet->num_bones = num_bones;
				skelet->bones = (TekBone*)tek_malloc(sizeof(TekBone) * num_bones);
			}
			else if (line[0] == 'b' && line[1] == 'o' && line[2] == 'n')
			{
				Vec3 pos;
				Quat rot;
				Vec3 size;
				int parent_id;
				char name[128];
				int result = sscanf(line, "hdr %f %f %f %f %f %f %f %f %f %f %d %s", &pos.x, &pos.y, &pos.z,
				                    &rot.x, &rot.y, &rot.z, &rot.w, &size.x, &size.y, &size.z, &parent_id, name);
				if (result < 1)
				{
					printf("Warning: cannot process line: %s File: %s\n", line, filename);
					fclose(fp);
					return false;
				}

				TekBone bone;
				bone.position = pos;
				bone.rotation = rot;
				bone.size = size;
				strcpy(bone.name,name);
				bone.parent_id = parent_id;
				bone.num_children = 0;
				skelet->bones[bone_ctr++] = bone;
			}
		}

		fclose(fp);

		//create bone hierarchy
		for (u32 i = 0; i < skelet->num_bones; ++i)
		{
			TekBone *bn = &skelet->bones[i];
			if (bn->parent_id < 0)
			{
				skelet->root_id = i;
			}
			else
			{
				TekBone *parent = &skelet->bones[bn->parent_id];
				parent_to(bn, parent);
			}
		}


		return true;
	}

	static void parent_to(TekBone *bone, TekBone *parent)
	{

	}

