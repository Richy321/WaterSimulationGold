#include "Plane.h"

namespace octet 
{
	/// Scene containing a box with octet.
	class water : public app 
	{
		// scene for drawing box
		ref<visual_scene> app_scene;
		ref<Plane> waterPlane;
		octet::camera_instance *camera; /// main camera instance 
		octet::mouse_look mouseLookHelper;

	public:
		/// this is called when we construct the class before everything is initialised.
		water(int argc, char **argv) : app(argc, argv)
		{
		}

		/// this is called once OpenGL is initialized
		void app_init()
		{
			app_scene = new visual_scene();
			app_scene->create_default_camera_and_lights();

			mouseLookHelper.init(this, 90.0f / 360.0f, false);

			//intiialise plane
			material *blue = new material(vec4(0, 0, 1, 1));
			const octet::vec3 size(100.0f, 0.0f, 100.0f);
			const octet::ivec3 dimensions(128, 0, 128);
			waterPlane = new Plane(dimensions, size);
			waterPlane->setArrays();

			scene_node *node = new scene_node();
			app_scene->add_child(node);
			app_scene->add_mesh_instance(new mesh_instance(node, waterPlane, blue));
			

			//change camera pos
			camera = app_scene->get_camera_instance(0);
			camera->set_far_plane(1000.0f);
			camera->get_node()->loadIdentity();
			camera->get_node()->rotate(-35, octet::vec3(1, 0, 0));
			camera->get_node()->translate(octet::vec3(size.x(), -size.z(), 400.0f));
		}

		/// this is called to draw the world
		void draw_world(int x, int y, int w, int h) 
		{
			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			
			//update simulations
			update();

			app_scene->begin_render(vx, vy);

			// update matrices. assume 30 fps.
			app_scene->update(1.0f / 30);

			// draw the scene
			app_scene->render((float)vx / vy);
		}

		void update()
		{
			octet::scene_node *camera_node = camera->get_node();
			octet::mat4t &camera_to_world = camera_node->access_nodeToParent();
			mouseLookHelper.update(camera_to_world);
			handleKeyboardControl();
		}

		void handleKeyboardControl()
		{
			if (is_key_down('Q')){
				app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, 2.5, 0);
			}
			else if (is_key_down('E')){
				app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, -2.5, 0);
			}
			else if (is_key_down('A')){
				app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(-2.5, 0, 0);
			}
			else if (is_key_down('D')){
				app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(2.5, 0, 0);
			}
			else if (is_key_down('W')){
				app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, 0, -2.5);
			}
			else if (is_key_down('S')){
				app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, 0, 2.5);
			}

			if (is_key_going_down('R'))
			{
				//terrain->usePerlinRandom = !terrain->usePerlinRandom;
			}

			if (is_key_going_down('C'))
			{
				//terrain->randomise = !terrain->randomise;
			}

			if (is_key_going_down('X'))
			{
				//if (terrain->octaves < 16)
				//	terrain->octaves = terrain->octaves * 2;
			}

			if (is_key_going_down('Z'))
			{
				//if (terrain->octaves > 1)
				//	terrain->octaves = terrain->octaves * 0.5f;
			}


			//Lighting Tests
			if (is_key_down('J')){
				app_scene->get_light_instance(0)->get_node()->access_nodeToParent().rotateY(1);
			}
			else if (is_key_down('L')){
				app_scene->get_light_instance(0)->get_node()->access_nodeToParent().rotateY(-1);
			}

			if (is_key_down('I')){
				app_scene->get_light_instance(0)->get_node()->access_nodeToParent().rotateX(1);
			}
			else if (is_key_down('K')){
				app_scene->get_light_instance(0)->get_node()->access_nodeToParent().rotateX(-1);
			}

			if (is_key_down(octet::key_esc))
			{
				exit(1);
			}
		}
	};
}
