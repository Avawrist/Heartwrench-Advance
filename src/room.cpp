#include "room.h"

/////////////////
// Struct Room //
/////////////////

Room::Room()
{

}

Room::Room(RoomName                                       _room_name, 
           bn::camera_ptr                                 camera_ptr,                
           const bn::regular_bg_ptr&                      object_bg_ptr, 
           const bn::regular_bg_item&                     object_bg_item,
           uint8                                          object_cells[LEVEL_OBJECT_CELL_WIDTH][LEVEL_OBJECT_CELL_HEIGHT],
           const bn::fixed_point                          player_spawn)
{
    load(_room_name, 
         camera_ptr,
         object_bg_ptr,
         object_bg_item,
         object_cells,
         player_spawn);
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

    room_name       = other.room_name;
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

    room_name       = other.room_name;
    top_neighbor    = other.top_neighbor;
    right_neighbor  = other.right_neighbor;
    bottom_neighbor = other.bottom_neighbor;
    left_neighbor   = other.left_neighbor;

    first_frame = other.first_frame;
    
}

int32 Room::addObject(ObjectRequest& object_request, const bn::camera_ptr& camera_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    GameObject* temp_object_ptr = NULL;

    bool priority_object = false;

    // Allocate object based on type
    // NOTE: All object types should be represented here. When adding object types
    //       this list must be updated.
    
    switch(object_request.object_type)
    {
        ///////////////////
	    // Level Objects //
	    ///////////////////

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

        case FALLING_PLATFORM_WIDE:
            temp_object_ptr = new FallingPlatformWide();
            priority_object = true;
        break;

        case FALLING_PLATFORM_THIN:
            temp_object_ptr = new FallingPlatformThin();
            priority_object = true;
        break;

        case PUSH_BLOCK:
            temp_object_ptr = new PushBlock();
            priority_object = true;
        break;

        case PUSH_BLOCK_MINI:
            temp_object_ptr = new PushBlockMini();
            priority_object = true;
        break;

        case AUTO_PLATFORM:
            temp_object_ptr = new AutoPlatform();
            priority_object = true;
        break;

        case SMASH_BLOCK_LARGE:
            temp_object_ptr = new SmashBlockLarge();
        break;

        case SMASH_BLOCK_MINI:
            temp_object_ptr = new SmashBlockMini();
        break;

        case SMASH_BLOCK_ZIGGURAT_L:
            temp_object_ptr = new SmashBlockZigguratL();
        break;

        case SMASH_BLOCK_ZIGGURAT_C:
            temp_object_ptr = new SmashBlockZigguratC();
        break;

        case SMASH_BLOCK_ZIGGURAT_R:
            temp_object_ptr = new SmashBlockZigguratR();
        break;

        case LARGE_VASE:
            temp_object_ptr = new LargeVase();
        break;

        case SMALL_VASE:
            temp_object_ptr = new SmallVase();
        break;

        case HP_TOTEM:
            temp_object_ptr = new HPTotem();
        break;

        case HP_DROP:
            temp_object_ptr = new HPDrop();
        break;

        case MOON_DROP:
            temp_object_ptr = new MoonDrop();
        break;

        case SKULL_DROP:
            temp_object_ptr = new SkullDrop();
        break;

        case STAR_DROP:
            temp_object_ptr = new StarDrop();
        break;

        case CHECKPOINT:
            temp_object_ptr = new Checkpoint();
        break;

        case FINISH_SEAL:
            temp_object_ptr = new FinishSeal();
        break;

        case BOUNCE_BELL:
            temp_object_ptr = new BounceBell();
        break;

        case AUTO_BOUNCE_BELL:
            temp_object_ptr = new AutoBounceBell();
        break;

        ///////////////////
	    // Level Enemies //
	    ///////////////////

        case GROUND_GHOUL:
            temp_object_ptr = new GroundGhoul();
        break;

        case THORN_COLUMN:
            temp_object_ptr = new ThornColumn();
        break;

        case THORN_BAR:
            temp_object_ptr = new ThornBar();
        break;

        ///////////
        // Props //
        ///////////

        case CANDELABRA:
            temp_object_ptr = new Candelabra();
        break;

        /////////////////////
	    // Special Objects //
	    /////////////////////

        case HITBOX_SPIN_1:
        case HITBOX_SPIN_2:
            BN_LOG("Loading object of type ", object_request.object_type);
            BN_LOG("is not supported by function Room::addObject");
        break;

        case PLAYER:
            temp_object_ptr = new Player();
        break;

        case NO_TYPE:
        default:
            BN_LOG("Failed to load object - No type provided.");
            return -1;
        break;
    }

    if(priority_object)
    {
        game_objects.push_back(temp_object_ptr);
        game_objects.back()->setCamera(camera_ptr);
        game_objects.back()->setPos(object_request.position);
        game_objects.back()->object_id = game_objects.size() - 1;

        object_request = ObjectRequest();

        return game_objects.back()->object_id;
    }
    else if(object_request.object_type == PLAYER)
    {
        bn::vector<GameObject*, MAX_GAME_OBJECTS>::iterator insert_index = game_objects.begin();

        game_objects.insert(insert_index, temp_object_ptr);
        (*insert_index)->setCamera(camera_ptr);
        (*insert_index)->setPos(object_request.position);
        (*insert_index)->object_id = 0;

        object_request = ObjectRequest();

        return (*insert_index)->object_id;
    }
    else
    {
        bn::vector<GameObject*, MAX_GAME_OBJECTS>::iterator insert_index = game_objects.begin();
        insert_index++;

        game_objects.insert(insert_index, temp_object_ptr);
        (*insert_index)->setCamera(camera_ptr);
        (*insert_index)->setPos(object_request.position);
        (*insert_index)->object_id = 1;

        updateIndexes();

        object_request = ObjectRequest();

        return (*insert_index)->object_id;
    }
}

int32 Room::addObject(const UnloadedObject& object, const bn::camera_ptr& camera_ptr)
{
    if(game_objects.size() >= MAX_GAME_OBJECTS) {return -1;}

    GameObject* temp_object_ptr = NULL;

    bool priority_object = false;
    bool _is_persistent  = false;

    // Allocate object based on type
    // NOTE: All object types should be represented here. When adding object types
    //       this list must be updated.
    
    switch(object.object_type)
    {
        ///////////////////
	    // Level Objects //
	    ///////////////////

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

        case FALLING_PLATFORM_WIDE:
            temp_object_ptr = new FallingPlatformWide();
            priority_object = true;
        break;

        case FALLING_PLATFORM_THIN:
            temp_object_ptr = new FallingPlatformThin();
            priority_object = true;
        break;

        case PUSH_BLOCK:
            temp_object_ptr = new PushBlock();
            priority_object = true;
        break;

        case PUSH_BLOCK_MINI:
            temp_object_ptr = new PushBlockMini();
            priority_object = true;
        break;

        case AUTO_PLATFORM:
            temp_object_ptr = new AutoPlatform();
            priority_object = true;
        break;

        case SMASH_BLOCK_LARGE:
            temp_object_ptr = new SmashBlockLarge();
            _is_persistent  = true;
        break;

        case SMASH_BLOCK_MINI:
            temp_object_ptr = new SmashBlockMini();
            _is_persistent  = true;
        break;

        case SMASH_BLOCK_ZIGGURAT_L:
            temp_object_ptr = new SmashBlockZigguratL();
            _is_persistent  = true;
        break;

        case SMASH_BLOCK_ZIGGURAT_C:
            temp_object_ptr = new SmashBlockZigguratC();
            _is_persistent  = true;
        break;

        case SMASH_BLOCK_ZIGGURAT_R:
            temp_object_ptr = new SmashBlockZigguratR();
            _is_persistent  = true;
        break;

        case LARGE_VASE:
            temp_object_ptr = new LargeVase();
            _is_persistent  = true;
        break;

        case SMALL_VASE:
            temp_object_ptr = new SmallVase();
            _is_persistent  = true;
        break;

        case HP_TOTEM:
            temp_object_ptr = new HPTotem();
            _is_persistent  = true;
        break;

        case HP_DROP:
            temp_object_ptr = new HPDrop();
            _is_persistent  = true;
            // Special Case: 
            // HP Drops added in through the Level Editor are "frozen"
            // and don't receive physics updates
            temp_object_ptr->is_frozen = true;
        break;

        case MOON_DROP:

            temp_object_ptr = new MoonDrop();
            _is_persistent  = true;
            // Special Case: 
            // Moon Drops added in through the Level Editor are "frozen"
            // and don't receive physics updates
            temp_object_ptr->is_frozen = true;

        break;

        case SKULL_DROP:

            temp_object_ptr = new SkullDrop();
            _is_persistent  = true;
            // Special Case: 
            // Skull Drops added in through the Level Editor are "frozen"
            // and don't receive physics updates
            temp_object_ptr->is_frozen = true;
            
        break;

        case STAR_DROP:

            temp_object_ptr = new StarDrop();
            _is_persistent  = true;
            // Special Case: 
            // Star Drops added in through the Level Editor are "frozen"
            // and don't receive physics updates
            temp_object_ptr->is_frozen = true;

        break;

        case CHECKPOINT:
            temp_object_ptr = new Checkpoint();
        break;

        case FINISH_SEAL:
            temp_object_ptr = new FinishSeal();
        break;

        case BOUNCE_BELL:
            temp_object_ptr = new BounceBell();
        break;

        case AUTO_BOUNCE_BELL:
            temp_object_ptr = new AutoBounceBell();
        break;

        ///////////////////
	    // Level Enemies //
	    ///////////////////

        case GROUND_GHOUL:
            temp_object_ptr = new GroundGhoul();
        break;

        case THORN_COLUMN:
            temp_object_ptr = new ThornColumn();
            _is_persistent  = true;
        break;

        case THORN_BAR:
            temp_object_ptr = new ThornBar();
            _is_persistent  = true;
        break;

        ///////////
        // Props //
        ///////////

        case CANDELABRA:
            temp_object_ptr = new Candelabra();
        break;

        /////////////////////
	    // Special Objects //
	    /////////////////////

        case HITBOX_SPIN_1:
        case HITBOX_SPIN_2:
            BN_LOG("Loading object of type ", object.object_type);
            BN_LOG("is not supported by function Room::addObject");
        break;

        case PLAYER:
            temp_object_ptr = new Player();
        break;

        case NO_TYPE:
        default:
            BN_LOG("Failed to load object - No type provided.");
            return -1;
        break;
    }
	
    if(priority_object)
    {
        game_objects.push_back(temp_object_ptr);
        game_objects.back()->setCamera(camera_ptr);
        game_objects.back()->setPos(object.room_pos);
        game_objects.back()->object_id = game_objects.size() - 1;
        game_objects.back()->is_persistent = _is_persistent;

        return game_objects.back()->object_id;
    }
    else
    {
        bn::vector<GameObject*, MAX_GAME_OBJECTS>::iterator insert_index = game_objects.begin();
        insert_index++;
        
        game_objects.insert(insert_index, temp_object_ptr);
        (*insert_index)->setCamera(camera_ptr);
        (*insert_index)->setPos(object.room_pos);
        (*insert_index)->object_id = 1;
        (*insert_index)->is_persistent = _is_persistent;

        updateIndexes();

        return (*insert_index)->object_id;
    }

}

int32 Room::addUnloadedObject(const UnloadedObject& new_object)
{
    if(unloaded_objects.size() >= MAX_UNLOADED_OBJECTS) {return 0;}

    unloaded_objects.push_back(new_object);

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

void Room::load(RoomName                                       _room_name, 
                const bn::camera_ptr&                          camera_ptr, 
                const bn::regular_bg_ptr&                      object_bg_ptr, 
                const bn::regular_bg_item&                     object_bg_item,
                uint8                                          object_cells[LEVEL_OBJECT_CELL_WIDTH][LEVEL_OBJECT_CELL_HEIGHT],
                const bn::fixed_point                          player_spawn)
{
    if(_room_name == NO_ROOM) {return;}

    room_name = _room_name;

    // Initialize Objects
    switch(room_name)
    {
        case ROOM_NAME_CARD:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.setTopBoundAC(LEVEL_HEIGHT / 2);
            room_bounds.setBottomBoundAC(LEVEL_HEIGHT / 2);
            room_bounds.setLeftBoundAC(LEVEL_WIDTH / 2);
            room_bounds.setRightBoundAC(LEVEL_WIDTH / 2);

            // Add any special objects //
            
        break;

        case ROOM_TITLE_SCREEN:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.setTopBoundAC(LEVEL_HEIGHT / 2);
            room_bounds.setBottomBoundAC(LEVEL_HEIGHT / 2);
            room_bounds.setLeftBoundAC(LEVEL_WIDTH / 2);
            room_bounds.setRightBoundAC(LEVEL_WIDTH / 2);

            // Add any special objects //
            
        break;

        case ROOM_TEST_1:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TEST_2;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.setTopBoundAC(320);
            room_bounds.setBottomBoundAC(768);
            room_bounds.setLeftBoundAC(16);
            room_bounds.setRightBoundAC(624);

            // Add any special objects //
            
        break;

        case ROOM_TEST_2:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TEST_1;

            room_bounds.setTopBoundAC(320);
            room_bounds.setBottomBoundAC(768);
            room_bounds.setLeftBoundAC(624);
            room_bounds.setRightBoundAC(992);

            // Add any special objects //
            
        break;

        default:

            BN_LOG("Room creation failed - RoomName not found.");
            return;

        break;
    }

    // Init Player. Player will always be updated last.
    ObjectRequest player_request(ObjectRequest(PLAYER, player_spawn));
    addObject(player_request, camera_ptr);

    // Add all object stubs 
    prepObjects(object_bg_ptr, 
                object_bg_item, 
                object_cells);
    
}

void Room::prepObjects(const bn::regular_bg_ptr&                      object_bg_ptr, 
                       const bn::regular_bg_item&                     object_bg_item,
                       uint8                                          object_cells[LEVEL_OBJECT_CELL_WIDTH][LEVEL_OBJECT_CELL_HEIGHT])
{
    int32 half_level_width_pixels  = object_bg_ptr.dimensions().width()  / 2;
	int32 half_level_height_pixels = object_bg_ptr.dimensions().height() / 2;
    
    for(int32 x  = room_bounds.left_bound  + half_level_width_pixels; 
              x <= room_bounds.right_bound + half_level_width_pixels; 
              x += TILE_WIDTH * 2)
    {
        for(int32 y  = room_bounds.top_bound    + half_level_height_pixels; 
                  y <= room_bounds.bottom_bound + half_level_height_pixels;
                  y += TILE_HEIGHT * 2)
        {
            bn::point cell_index = bn::point((x / TILE_WIDTH) / 2, (y / TILE_HEIGHT) / 2);
            ObjectType type = (ObjectType)getDynamicTileAtBGIndex(cell_index.x(), 
                                                                  cell_index.y(),
                                                                  object_cells);

            if(type > NO_TYPE && type < HITBOX_SPIN_1)
            {
                int32 x_pos_offset = 0;
                int32 y_pos_offset = 0;
                
                // Special case objects with unique positioning:
                if(type == PUSH_BLOCK_MINI  ||
                   type == SMASH_BLOCK_MINI ||
                   type == AUTO_PLATFORM)
                {
                    x_pos_offset = TILE_WIDTH;
                    y_pos_offset = TILE_HEIGHT;
                }

                addUnloadedObject(UnloadedObject(bn::point(x - half_level_width_pixels  + x_pos_offset, 
                                                           y - half_level_height_pixels + y_pos_offset - 1), type));
            }
        }
    }
}

void Room::monitorObjectRequests(const bn::camera_ptr& camera_ptr)
{
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {
        if(game_objects.at(i)->object_request.object_type != NO_TYPE)
        {
            // Add the requested object
            addObject(game_objects.at(i)->object_request, camera_ptr);
        }
    }
}

void Room::monitorUnloadedObjects(const bn::camera_ptr& camera_ptr)
{
    if(game_objects.at(PLAYER_OBJECT_LIST_INDEX) == NULL) {return;}

    bn::fixed_point  camera_center = camera_ptr.position();
    if(first_frame) {camera_center = game_objects.at(PLAYER_OBJECT_LIST_INDEX)->pos();}

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

        load_range_collider = Collider(camera_center.x(), camera_center.y(), LOAD_RANGE_W * 2, LOAD_RANGE_H * 2);

        // Look at all unloaded objects in the list. If the position is within the defined
        // camera boundaries, actually load the object.
        for(int i = 0; i < unloaded_objects.size(); i++)
        {
            // If object is within the load window:
            if(load_range_collider.isCollision(unloaded_objects.at(i).room_pos)) // <-----
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

void Room::updateIndexes()
{    
    for(int32 i = game_objects.size() - 1; i >= 0; i--)
    {
        // Search for the unloaded object by index, if found, update the ID:
        int32 object_id = game_objects.at(i)->object_id;
        int32 unloaded_index = findUnloadedObjectIndex(object_id);
        if(unloaded_index > -1)
        {unloaded_objects.at(unloaded_index).loaded_instance_id = i;}

        // Update the loaded object's ID:
        game_objects.data()[i]->object_id = i;
    }
}

bn::point Room::center()
{
    return(room_bounds.center());
}