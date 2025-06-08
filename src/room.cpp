#include "room.h"

/////////////////
// Struct Room //
/////////////////

Room::Room()
{

}

Room::Room(RoomName                                       room_name, 
           bn::camera_ptr                                 camera_ptr,                
           const bn::regular_bg_ptr&                      object_bg_ptr, 
           const bn::regular_bg_item&                     object_bg_item,
           const bn::span<const bn::regular_bg_map_cell>& object_cells)
{
    load(room_name, 
         camera_ptr,
         object_bg_ptr,
         object_bg_item,
         object_cells);
}

Room::Room(const Room& other)
{

    for(int i = 0; i < game_objects.size(); i++)
    {
        delete game_objects.at(i);
    }
    game_objects.clear();

    for(int i = 0; i < other.game_objects.size(); i++)
    {
        switch(other.game_objects.at(i)->object_type)
        {
            case PLAYER:
                game_objects.push_back(new Player(*((Player*)(other.game_objects.at(i)))));
            break;

            default:
            break;
        }
    }

    // Copy unloaded objects
    unloaded_objects = other.unloaded_objects;

    room_bounds = other.room_bounds;

    top_neighbor    = other.top_neighbor;
    right_neighbor  = other.right_neighbor;
    bottom_neighbor = other.bottom_neighbor;
    left_neighbor   = other.left_neighbor;

    first_frame = other.first_frame;

}

Room::~Room()
{
    clear();
}

void Room::operator =(const Room& other)
{

    for(int i = 0; i < game_objects.size(); i++)
    {
        delete game_objects.at(i);
    }
    game_objects.clear();

    for(int i = 0; i < other.game_objects.size(); i++)
    {
        switch(other.game_objects.at(i)->object_type)
        {
            case PLAYER:
                game_objects.push_back(new Player(*((Player*)(other.game_objects.at(i)))));
            break;

            default:
            break;
        }
    }

    // Copy unloaded objects
    unloaded_objects = other.unloaded_objects;

    room_bounds = other.room_bounds;

    top_neighbor    = other.top_neighbor;
    right_neighbor  = other.right_neighbor;
    bottom_neighbor = other.bottom_neighbor;
    left_neighbor   = other.left_neighbor;

    first_frame = other.first_frame;
    
}

int32 Room::addObject(GameObject* object_ptr, const bn::camera_ptr& camera_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    game_objects.push_back(object_ptr);
    game_objects.back()->setCamera(camera_ptr);
    game_objects.back()->object_id = game_objects.size() - 1;

    return game_objects.back()->object_id;
}

int32 Room::addObject(const UnloadedObject& object, const bn::camera_ptr& camera_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    GameObject* temp_object_ptr = NULL;

    // Allocate object based on type
    // NOTE: All object types should be represented here. When adding object types 
    //       this list must be updated.
    
    switch(object.object_type)
    {
        case PLAYER:
            temp_object_ptr = new Player();
        break;

        case GROUND_GHOUL:
            temp_object_ptr = new GroundGhoul();
        break;

        case CEILING_GHOUL:
            temp_object_ptr = new CeilingGhoul();
        break;

        case WALL_LEFT_GHOUL:
            temp_object_ptr = new WallLeftGhoul();
        break;

        case WALL_RIGHT_GHOUL:
            temp_object_ptr = new WallRightGhoul();
        break;

        case PHASE_ORB_UP:
            temp_object_ptr = new PhaseOrbUp();
        break;

        case PHASE_ORB_DOWN:
            temp_object_ptr = new PhaseOrbDown();
        break;

        case PHASE_ORB_LEFT:
            temp_object_ptr = new PhaseOrbLeft();
        break;

        case PHASE_ORB_RIGHT:
            temp_object_ptr = new PhaseOrbRight();
        break;

        case TILE_PASSAGE:
            temp_object_ptr = new TilePassage();
        break;

        case THORN_COLUMN:
            temp_object_ptr = new ThornColumn();
        break;

        case THORN_BAR:
            temp_object_ptr = new ThornBar();
        break;

        case FALLING_PLATFORM_WIDE:
            temp_object_ptr = new FallingPlatformWide();
        break;

        case DEVIL_PLATFORM:
        case ANGEL_PLATFORM:
        case SCYTHE_PLATFORM:
        case HITBOX_ATTACK_GROUND_1:
        case HITBOX_ATTACK_AIR_1:
            BN_LOG("Loading object of type ", object.object_type);
            BN_LOG("is not supported by function Room::addObject");
        break;

        case NO_TYPE:
        default:
            BN_LOG("Failed to load object - No type provided.");
            return -1;
        break;
    }
	
    game_objects.push_back(temp_object_ptr);
    game_objects.back()->setCamera(camera_ptr);
    game_objects.back()->setPos(object.room_pos);
    game_objects.back()->object_id = game_objects.size() - 1;
    game_objects.back()->is_persistent = object.is_persistent;

    return game_objects.back()->object_id;
}

int32 Room::addUnloadedObject(const UnloadedObject& new_object, bool is_persistent)
{
    if(unloaded_objects.size() >= MAX_UNLOADED_OBJECTS) {return 0;}

    unloaded_objects.push_back(new_object);
    unloaded_objects.back().is_persistent = is_persistent;

    return 1;
}

int32 Room::findUnloadedObjectIndex(int32 object_id)
{
    for(int32 i = 0; i < unloaded_objects.size(); i++)
    {
        if(unloaded_objects.at(i).loaded_instance_id == object_id)
        {
            return i;
        }
    }

    return -1;
}

void Room::clear()
{

    // Free all game object pointers
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {
        delete game_objects.at(i);
    }

    // Remove all game objects from vector
    game_objects.clear();

}

void Room::load(RoomName                                       room_name, 
                const bn::camera_ptr&                          camera_ptr, 
                const bn::regular_bg_ptr&                      object_bg_ptr, 
                const bn::regular_bg_item&                     object_bg_item,
                const bn::span<const bn::regular_bg_map_cell>& object_cells)
{
    if(room_name == NO_ROOM) {return;}

    // Init Player FIRST. Player will always be updated last.
    Player* player_ptr = new Player();
    addObject(player_ptr, camera_ptr);
    game_objects.back()->setPos(-4960, -2096);

    // Initialize Objects
    switch(room_name)
    {
        case ROOM_TEST_1:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TEST_2;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.top_bound    = -2428;
            room_bounds.right_bound  = -4784;
            room_bounds.bottom_bound = -2064;
            room_bounds.left_bound   = -5120;

            // Add any special objects //
            
        break;

        case ROOM_TEST_2:

            // Init Variables //
            top_neighbor    = ROOM_TEST_4;
            right_neighbor  = ROOM_TEST_3;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TEST_1;

            room_bounds.top_bound    = -2256;
            room_bounds.right_bound  = -4368;
            room_bounds.bottom_bound = -2064;
            room_bounds.left_bound   = -4784;

            // Add any special objects //
    
        break;

        case ROOM_TEST_3:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TEST_2;

            room_bounds.top_bound    = -2428;
            room_bounds.right_bound  = -4000;
            room_bounds.bottom_bound = -2064;
            room_bounds.left_bound   = -4368;

            // Add any special objects //
    
        break;

        case ROOM_TEST_4:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = ROOM_TEST_2;
            left_neighbor   = NO_ROOM;

            room_bounds.top_bound    = -2432;
            room_bounds.right_bound  = -4368;
            room_bounds.bottom_bound = -2256;
            room_bounds.left_bound   = -4784;

            // Add any special objects //
    
        break;

        default:

            BN_LOG("Room creation failed - RoomName not found.");
            return;

        break;
    }

    // Add all object stubs 
    prepObjects(object_bg_ptr, 
                object_bg_item, 
                object_cells);

}

void Room::prepObjects(const bn::regular_bg_ptr&                      object_bg_ptr, 
                       const bn::regular_bg_item&                     object_bg_item,
                       const bn::span<const bn::regular_bg_map_cell>& object_cells)
{
    
    int32 half_level_width_pixels  = object_bg_ptr.dimensions().width()  / 2;
	int32 half_level_height_pixels = object_bg_ptr.dimensions().height() / 2;
    
    for(int32 x = room_bounds.left_bound   + half_level_width_pixels + (TILE_WIDTH / 2); 
              x <= room_bounds.right_bound + half_level_width_pixels - (TILE_WIDTH / 2); 
              x += TILE_WIDTH)
    {
        for(int32 y = room_bounds.top_bound     + half_level_height_pixels + (TILE_HEIGHT / 2); 
                  y <= room_bounds.bottom_bound + half_level_height_pixels - (TILE_HEIGHT / 2);
                  y += TILE_HEIGHT)
        {
            bn::point cell_index = bn::point((x / TILE_WIDTH), (y / TILE_HEIGHT));
            ObjectType type = (ObjectType)getTileAtBGIndex(cell_index.x(), 
                                                           cell_index.y(), 
                                                           object_bg_ptr,
                                                           object_cells,
                                                           object_bg_item);

            if(type > NO_TYPE && type < DEVIL_PLATFORM)
            {
                addUnloadedObject(UnloadedObject(bn::point(x - half_level_width_pixels  + (TILE_WIDTH / 2), 
                                                           y - half_level_height_pixels + (TILE_HEIGHT / 2) - 1), 
                                                           type), 
                                  false);
            }
        }
    }
    
}

void Room::monitorUnloadedObjects(const bn::camera_ptr& camera_ptr)
{
    #define SCREEN_LOAD_PADDING 64

    if(game_objects.at(PLAYER_OBJECT_LIST_INDEX) == NULL) {return;}

    bn::fixed_point camera_center = game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos();
    Collider load_range_collider(camera_center.x(), 
                                 camera_center.y(), 
                                 LOAD_RANGE_W, 
                                 LOAD_RANGE_H);
    Collider screen_range_collider(camera_center.x(), 
                                   camera_center.y(), 
                                   SCREEN_W + SCREEN_LOAD_PADDING, 
                                   SCREEN_H + SCREEN_LOAD_PADDING);

    if(first_frame)
    {
        first_frame = false;

        // Look at all unloaded objects in the list. If the position is within the defined
        // camera boundaries, actually load the object.
        for(int i = 0; i < unloaded_objects.size(); i++)
        {
            // If object is within the load window:
            if(load_range_collider.isCollision(unloaded_objects.at(i).room_pos))
            {
                // If there is not already a loaded instance, load one in:
                if(unloaded_objects.at(i).loaded_instance_id == UNLOADED_OBJECT_STATE_UNLOADED)
                {
                    unloaded_objects.at(i).loaded_instance_id = addObject(unloaded_objects.at(i),
                                                                          camera_ptr);
                }
            }
        }
    }
    else
    {
        // Look at all unloaded objects in the list. If the position is within the defined
        // camera boundaries, actually load the object.
        for(int i = 0; i < unloaded_objects.size(); i++)
        {
            // If object is within the load window:
            if(load_range_collider.isCollision(unloaded_objects.at(i).room_pos))
            {
                // If object is outside of camera window (don't load stuff on screen):
                if(!screen_range_collider.isCollision(unloaded_objects.at(i).room_pos))
                {
                    // If there is not already a loaded instance, load one in:
                    if(unloaded_objects.at(i).loaded_instance_id == UNLOADED_OBJECT_STATE_UNLOADED)
                    {
                        unloaded_objects.at(i).loaded_instance_id = addObject(unloaded_objects.at(i),
                                                                              camera_ptr);
                    }
                }
            }
        }
    }

}