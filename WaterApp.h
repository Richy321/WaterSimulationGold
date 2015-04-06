#include "Plane.h"
#include "WaterPlane.h"

#include <iostream>
#include <fstream>
#include <iomanip>

namespace octet 
{
	/// Scene containing a box with octet.
	class water : public app 
	{
		// scene for drawing box
		ref<visual_scene> app_scene;
		WaterPlane* waterPlane;
		octet::camera_instance *camera; /// main camera instance 
		octet::mouse_look mouseLookHelper;
		bool isShowToolbar = true;
		TwBar* toolbar;

		char buff[1];
		
		string configurationPath = "config.cfg";
		string getUrlPath = "src/examples/water/";

	public:
		/// this is called when we construct the class before everything is initialised.
		water(int argc, char **argv) : app(argc, argv)
		{
		}

		~water()
		{
			//Un-Initialise toolbar
			TwTerminate();
		}

		/// this is called once OpenGL is initialized
		void app_init()
		{
			app_scene = new visual_scene();
			app_scene->create_default_camera_and_lights();

			//add some more lights (looks a bit better)
			light *dirLight1 = new light();
			scene_node *lightNode1 = new scene_node();
			light_instance *dirLightInstance1 = new light_instance();
			lightNode1->access_nodeToParent().translate(100, 100, 100);
			lightNode1->access_nodeToParent().rotateX(35);
			lightNode1->access_nodeToParent().rotateY(135);
			dirLight1->set_color(vec4(1, 1, 1, 1));
			dirLight1->set_kind(atom_directional);
			dirLightInstance1->set_node(lightNode1);
			dirLightInstance1->set_light(dirLight1);
			app_scene->add_light_instance(dirLightInstance1);

			add_light_instances();
			mouseLookHelper.init(this, 90.0f / 360.0f, false);
			enable_cursor();

			//initialise wavePlane
			const octet::vec3 size(100.0f, 0.0f, 100.0f);
			const octet::ivec3 dimensions(400, 0, 400);
			waterPlane = new WaterPlane(dimensions, size);

			scene_node *node = new scene_node();
			app_scene->add_child(node);
			app_scene->add_mesh_instance(new mesh_instance(node, waterPlane->getMesh(), waterPlane->getMaterial()));
			
			//change camera pos
			camera = app_scene->get_camera_instance(0);
			camera->set_far_plane(1000.0f);
			camera->get_node()->loadIdentity();
			camera->get_node()->rotate(-35, octet::vec3(1, 0, 0));
			camera->get_node()->translate(octet::vec3(size.x(), -size.z(), 400.0f));

			waterPlane->waveCount;

			string path(getUrlPath);
			path += configurationPath;
			LoadConfigurationFile(path.c_str(), &waterPlane->waves);


			InitialiseToolbar();
		}

		/// this is called to draw the world
		void draw_world(int x, int y, int w, int h) 
		{
			int vx = 0, vy = 0;
			get_viewport_size(vx, vy);
			
			TwWindowSize(vx, vy);

			// update simulations
			update();

			app_scene->begin_render(vx, vy);

			// update matrices. assume 30 fps.
			app_scene->update(1.0f / 30);

			// draw the scene
			app_scene->render((float)vx / vy);

			// draw Toolbar
			if(isShowToolbar)
				TwDraw();
		}

		void update()
		{
			handleKeyboardControl();
			handleMouseControl();

			waterPlane->Update();
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
			else if (!is_key_down(octet::key_ctrl) && is_key_down('S')){
				app_scene->get_camera_instance(0)->get_node()->access_nodeToParent().translate(0, 0, 2.5);
			}

			if (is_key_going_down('1'))
			{
				waterPlane->toggleWireframe();
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

			if (is_key_down(octet::key_ctrl) && is_key_going_down('S'))
			{
				SaveConfiguration(configurationPath.c_str(), &waterPlane->waves);
			}

			if (is_key_down(octet::key_ctrl) && is_key_going_down('L'))
			{
				string path(getUrlPath);
				path += configurationPath;
				LoadConfigurationFile(path.c_str(), &waterPlane->waves);
			}

			if (is_key_down(octet::key_esc))
			{
				exit(1);
			}
		}

		void handleMouseControl()
		{
			//Pass lmb mouse events to toolbar
			int x = 0;
			int y = 0;
			get_mouse_pos(x, y);
			TwMouseMotion(x, y);
			if (is_key_going_down(key_lmb))
			{
				TwMouseButton(TW_MOUSE_PRESSED, TW_MOUSE_LEFT);
			}

			if (!is_key_down(key_lmb) && get_prev_keys()[key_lmb] != 0)
			{
				TwMouseButton(TW_MOUSE_RELEASED, TW_MOUSE_LEFT);
			}

			int rx = 0, ry = 0;
			get_absolute_mouse_movement(rx, ry);

			//only update mouse with alt held (allows clicking UI)
			if (is_key_going_down(octet::key_alt))
			{
				accumulate_absolute_mouse_movement(-rx, -ry);
			}

			if (is_key_down(octet::key_alt))
			{
				//disable_cursor();
				octet::mat4t &camera_to_world = camera->get_node()->access_nodeToParent();
				mouseLookHelper.update(camera_to_world);
			}
			else if (is_key_going_up(octet::key_alt))
			{
				//enable_cursor();
				mouseLookHelper.set_mouse_center(rx, ry);
			}
		}

		void InitialiseToolbar()
		{
			TwInit(TW_OPENGL, NULL);
			toolbar = TwNewBar("WaveParameters");
			TwDefine(" WaveParameters label='Configuration' ");
			TwAddVarRW(toolbar, "Water Colour", TW_TYPE_COLOR3F, &waterPlane->colour, " label='Water Colour' ");
			TwAddVarRW(toolbar, "Water Alpha", TW_TYPE_FLOAT, &waterPlane->alpha, " label='Water Alpha' step=0.01 min=0.0 max=1.0");
			TwAddVarRW(toolbar, "Wave Count", TW_TYPE_INT32, &waterPlane->waveCount, " label='Wave Count' min=1 max=8 ");
			TwAddVarRW(toolbar, "Wave Type", TW_TYPE_INT32, &waterPlane->waveType, " label='Wave Type' min=0 max=1");

			string stringQuote = "'";
			string stepText = " step=0.01 ";
			
			for (int i = 0; i < waterPlane->waveCount; i++)
			{
				sprintf(buff, "%d", i);
				string groupText = " group='Wave";
				groupText += buff;
				groupText += stringQuote;
				groupText += stepText;
				
				TwAddVarRW(toolbar, string("Amplitude ") += buff, TW_TYPE_FLOAT, &waterPlane->waves[i]->amplitude, groupText);
				TwAddVarRW(toolbar, string("Wavelength ") += buff, TW_TYPE_FLOAT, &waterPlane->waves[i]->wavelength, groupText);
				TwAddVarRW(toolbar, string("Speed ") += buff, TW_TYPE_FLOAT, &waterPlane->waves[i]->speed, groupText);
				TwAddVarRW(toolbar, string("Steepness ") += buff, TW_TYPE_FLOAT, &waterPlane->waves[i]->steepness, groupText);
				TwAddVarRW(toolbar, string("Direction X ") += buff, TW_TYPE_FLOAT, &waterPlane->waves[i]->directionX, groupText);
				TwAddVarRW(toolbar, string("Direction Y ") += buff, TW_TYPE_FLOAT, &waterPlane->waves[i]->directionY, groupText);
			}
		}

		void ToggleToolbar(bool isShow)
		{
			isShowToolbar = isShow;
		}

		void LoadConfigurationFile(const char *path, octet::dynarray<wave*>* waves)
		{
			//read while file into memory (quicker than line by line)
			dynarray<uint8_t> fileContents;
			app_utils::get_url(fileContents, path);

			char* curPos = (char*)fileContents.data();
			waterPlane->waveType = (WaterPlane::WavesType)(int)atoi(curPos);
			curPos += 3;
			waterPlane->waveCount = (int)atoi(curPos);
			curPos += 3;
			waterPlane->alpha = (float)atof(curPos);
			
			char* tokens = strtok(curPos, " \r\n");
			tokens = strtok(NULL, " \r\n");
			
			for (int i = 0; i < 8; i++)
			{
				(*waves)[i]->amplitude = (float)atof(tokens); tokens = strtok(NULL, " ");
				(*waves)[i]->wavelength = (float)atof(tokens); tokens = strtok(NULL, " ");
				(*waves)[i]->speed = (float)atof(tokens); tokens = strtok(NULL, " ");
				(*waves)[i]->steepness = (float)atof(tokens); tokens = strtok(NULL, " ");
				(*waves)[i]->directionX = (float)atof(tokens); tokens = strtok(NULL, " \r\n");
				(*waves)[i]->directionY = (float)atof(tokens); tokens = strtok(NULL, " \r\n");
			}
		}

		void SaveConfiguration(const char *path, octet::dynarray<wave*>* waves)
		{
			std::ofstream configFile;
			
			int precision = 5;
			configFile.open(path);
			if (configFile.good())
			{
				configFile << waterPlane->waveType << "\n";
				configFile << waterPlane->waveCount << "\n";
				configFile << waterPlane->alpha << "\n";

				for (int i = 0; i < 8; i++)
				{
					configFile << std::setprecision(precision) << (*waves)[i]->amplitude << " ";
					configFile << std::setprecision(precision) << (*waves)[i]->wavelength << " ";
					configFile << std::setprecision(precision) << (*waves)[i]->speed << " ";
					configFile << std::setprecision(precision) << (*waves)[i]->steepness << " ";
					configFile << std::setprecision(precision) << (*waves)[i]->directionX << " ";
					configFile << std::setprecision(precision) << (*waves)[i]->directionY << "\n";
				}
			}
			configFile.close();
		}


		void add_light_instances(){
			//this one works 
			light *_light = new light();
			light_instance *li = new light_instance();
			scene_node *node = new scene_node();
			app_scene->add_child(node);
			node->translate(vec3(0.0f, -100, -100));
			node->rotate(-45, vec3(1, 0, 0));
			node->rotate(-180, vec3(0, 1, 0));
			_light->set_color(vec4(1, 1, 1, 1));
			_light->set_kind(atom_directional);
			li->set_node(node);
			li->set_light(_light);
			app_scene->add_light_instance(li);

			node = new scene_node();
			app_scene->add_child(node);
			_light = new light();
			li = new light_instance();
			node->translate(vec3(-100, 100, -100));
			node->rotate(-45, vec3(1, 0, 0));
			node->rotate(45, vec3(0, 1, 0));
			_light->set_color(vec4(1, 1, 1, 1));
			_light->set_kind(atom_directional);
			li->set_node(node);
			li->set_light(_light);
			app_scene->add_light_instance(li);
		}
	};
}
