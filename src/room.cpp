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
           uint8**                                        object_cells,
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

        case STAR_JAR:
            temp_object_ptr = new StarJar();
        break;

        case SKULL_DROP:
            temp_object_ptr = new SkullDrop();
        break;

        case GEAR_DROP:
            temp_object_ptr = new GearDrop();
        break;

        case SCREW:
            temp_object_ptr = new GearDrop();
            priority_object = true;
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

        case THORN_COLUMN:
            temp_object_ptr = new ThornColumn();
        break;

        case THORN_BAR:
            temp_object_ptr = new ThornBar();
        break;

        case GROUND_GHOUL:
            temp_object_ptr = new GroundGhoul();
        break;

        case BELL_TROLL:
            temp_object_ptr = new BellTroll();
        break;

        case WINGED_TROLL_L:
            temp_object_ptr = new WingedTrollL();
        break;

        case WINGED_TROLL_R:
            temp_object_ptr = new WingedTrollR();
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

        if(room_name != ROOM_NAME_CARD && 
           room_name != ROOM_TITLE_SCREEN && 
           room_name != ROOM_OVERWORLD)
        (*insert_index)->setState(PLAYER_ENTRANCE);

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

        case STAR_JAR:

            // Don't add the Star if it's already been collected.
            if(!starIsCollected())
            {
                temp_object_ptr = new StarJar();
                _is_persistent  = true;

                // Special Case: 
                // Star Drops added in through the Level Editor are "frozen"
                // and don't receive physics updates
                temp_object_ptr->is_frozen = true;
            }
            else
            {
                return -1;
            }

        break;

        case SKULL_DROP:

            temp_object_ptr = new SkullDrop();
            _is_persistent  = true;
            // Special Case: 
            // Skull Drops added in through the Level Editor are "frozen"
            // and don't receive physics updates
            temp_object_ptr->is_frozen = true;
            
        break;

        case GEAR_DROP:

            temp_object_ptr = new GearDrop();
            _is_persistent  = true;
            // Special Case: 
            // Gear Drops added in through the Level Editor are "frozen"
            // and don't receive physics updates
            temp_object_ptr->is_frozen = true;

        break;

        case SCREW:

            temp_object_ptr = new Screw();
            priority_object = true;
            _is_persistent   = true;

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

        case THORN_COLUMN:
            temp_object_ptr = new ThornColumn();
            _is_persistent  = true;
        break;

        case THORN_BAR:
            temp_object_ptr = new ThornBar();
            _is_persistent  = true;
        break;

        case GROUND_GHOUL:
            temp_object_ptr = new GroundGhoul();
        break;

        case BELL_TROLL:
            temp_object_ptr = new BellTroll();
        break;

        case WINGED_TROLL_L:
            temp_object_ptr = new WingedTrollL();
        break;

        case WINGED_TROLL_R:
            temp_object_ptr = new WingedTrollR();
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
                uint8**                                        object_cells,
                const bn::fixed_point                          player_spawn)
{
    if(_room_name == NO_ROOM) {return;}

    room_name = _room_name;

    // Initialize Room bounds
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

        case ROOM_OVERWORLD:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.top_bound    = ((OW_WIDTH / 2) - TILE_HEIGHT - 1) * -1;
            room_bounds.bottom_bound = (OW_WIDTH / 2) - TILE_HEIGHT - 1;
            room_bounds.left_bound   = ((OW_WIDTH / 2) - TILE_WIDTH - 1) * -1;
            room_bounds.right_bound  = (OW_WIDTH / 2) - TILE_WIDTH - 1;

            // Add any special objects //

        break;

        case ROOM_TROLL_TOLLS_1:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_2;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.setTopBoundAC(2224);
            room_bounds.setBottomBoundAC(2544);
            room_bounds.setLeftBoundAC(160);
            room_bounds.setRightBoundAC(752);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_2:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_3;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_1;

            room_bounds.setTopBoundAC(2224);
            room_bounds.setBottomBoundAC(2544);
            room_bounds.setLeftBoundAC(752);
            room_bounds.setRightBoundAC(1184);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_3:

            // Init Variables //
            top_neighbor    = ROOM_TROLL_TOLLS_4;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_2;

            room_bounds.setTopBoundAC(2224);
            room_bounds.setBottomBoundAC(2544);
            room_bounds.setLeftBoundAC(1184);
            room_bounds.setRightBoundAC(1664);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_4:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_5;
            bottom_neighbor = ROOM_TROLL_TOLLS_3;
            left_neighbor   = NO_ROOM;

            room_bounds.setTopBoundAC(2048);
            room_bounds.setBottomBoundAC(2224);
            room_bounds.setLeftBoundAC(1152);
            room_bounds.setRightBoundAC(1568);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_5:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_6;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_4;

            room_bounds.setTopBoundAC(1888);
            room_bounds.setBottomBoundAC(2320);
            room_bounds.setLeftBoundAC(1568);
            room_bounds.setRightBoundAC(1936);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_6:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_7;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_5;

            room_bounds.setTopBoundAC(1888);
            room_bounds.setBottomBoundAC(2320);
            room_bounds.setLeftBoundAC(1936);
            room_bounds.setRightBoundAC(2480);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_7:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_8;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_6;

            room_bounds.setTopBoundAC(1888);
            room_bounds.setBottomBoundAC(2320);
            room_bounds.setLeftBoundAC(2480);
            room_bounds.setRightBoundAC(2720);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_8:

            // Init Variables //
            top_neighbor    = ROOM_TROLL_TOLLS_9;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_7;

            room_bounds.setTopBoundAC(1904);
            room_bounds.setBottomBoundAC(2320);
            room_bounds.setLeftBoundAC(2720);
            room_bounds.setRightBoundAC(3216);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_9:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = ROOM_TROLL_TOLLS_8;
            left_neighbor   = ROOM_TROLL_TOLLS_10;

            room_bounds.setTopBoundAC(1280);
            room_bounds.setBottomBoundAC(1904);
            room_bounds.setLeftBoundAC(2720);
            room_bounds.setRightBoundAC(3216);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_10:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_9;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_11;

            room_bounds.setTopBoundAC(1280);
            room_bounds.setBottomBoundAC(1552);
            room_bounds.setLeftBoundAC(2448);
            room_bounds.setRightBoundAC(2720);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_11:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_10;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_12;

            room_bounds.setTopBoundAC(1280);
            room_bounds.setBottomBoundAC(1552);
            room_bounds.setLeftBoundAC(2176);
            room_bounds.setRightBoundAC(2448);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_12:

            // Init Variables //
            top_neighbor    = ROOM_TROLL_TOLLS_13;
            right_neighbor  = ROOM_TROLL_TOLLS_11;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = NO_ROOM;

            room_bounds.setTopBoundAC(1120);
            room_bounds.setBottomBoundAC(1552);
            room_bounds.setLeftBoundAC(1616);
            room_bounds.setRightBoundAC(2176);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_13:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_14;
            bottom_neighbor = ROOM_TROLL_TOLLS_12;
            left_neighbor   = NO_ROOM;

            room_bounds.setTopBoundAC(400);
            room_bounds.setBottomBoundAC(1120);
            room_bounds.setLeftBoundAC(1616);
            room_bounds.setRightBoundAC(2448);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_14:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_15;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_13;

            room_bounds.setTopBoundAC(400);
            room_bounds.setBottomBoundAC(864);
            room_bounds.setLeftBoundAC(2448);
            room_bounds.setRightBoundAC(2976);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_15:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = ROOM_TROLL_TOLLS_16;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_14;

            room_bounds.setTopBoundAC(16);
            room_bounds.setBottomBoundAC(864);
            room_bounds.setLeftBoundAC(2976);
            room_bounds.setRightBoundAC(3264);

            // Add any special objects //
            
        break;

        case ROOM_TROLL_TOLLS_16:

            // Init Variables //
            top_neighbor    = NO_ROOM;
            right_neighbor  = NO_ROOM;
            bottom_neighbor = NO_ROOM;
            left_neighbor   = ROOM_TROLL_TOLLS_15;

            room_bounds.setTopBoundAC(16);
            room_bounds.setBottomBoundAC(768);
            room_bounds.setLeftBoundAC(3264);
            room_bounds.setRightBoundAC(5088);

            // Add any special objects //
            
        break;

        default:

            BN_LOG("Room creation failed - RoomName not found.");
            return;

        break;
    }

    // Init Player. Player will always be updated last.
    ObjectRequest player_request(PLAYER, player_spawn);
    addObject(player_request, camera_ptr);

    // Add all object stubs 
    prepObjects(object_bg_ptr, 
                object_bg_item, 
                object_cells);
    
}

void Room::prepObjects(const bn::regular_bg_ptr&                      object_bg_ptr, 
                       const bn::regular_bg_item&                     object_bg_item,
                       uint8**                                        object_cells)
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

void Room::setStarCollected()
{
    switch(room_name)
    {   
        case ROOM_NAME_CARD:
        break;

        case ROOM_TITLE_SCREEN:
        break;

        case ROOM_OVERWORLD:
        break;

        case ROOM_TROLL_TOLLS_1:            
        break;

        case ROOM_TROLL_TOLLS_2:
        break;

        case ROOM_TROLL_TOLLS_3:

            global_troll_tolls_room_3_star = true;

        break;

        case ROOM_TROLL_TOLLS_4:
        break;

        case ROOM_TROLL_TOLLS_5:

            global_troll_tolls_room_5_star = true;

        break;

        case ROOM_TROLL_TOLLS_6:

            global_troll_tolls_room_6_star = true;

        break;

        case ROOM_TROLL_TOLLS_7:

            global_troll_tolls_room_7_star = true;

        break;

        case ROOM_TROLL_TOLLS_8:
        break;

        case ROOM_TROLL_TOLLS_9:

            global_troll_tolls_room_9_star = true;

        break;

        case ROOM_TROLL_TOLLS_10:
        break;

        case ROOM_TROLL_TOLLS_11:
        break;

        case ROOM_TROLL_TOLLS_12:
        break;

        case ROOM_TROLL_TOLLS_13:
        
            global_troll_tolls_room_13_star = true;

        break;

        case ROOM_TROLL_TOLLS_14:
        break;

        case ROOM_TROLL_TOLLS_15:
        break;

        case ROOM_TROLL_TOLLS_16:
        break;

        default:
        break;
    }  
}

bool Room::starIsCollected()
{
    bool is_collected = false;

    switch(room_name)
    {   
        case ROOM_NAME_CARD:
        break;

        case ROOM_TITLE_SCREEN:
        break;

        case ROOM_OVERWORLD:
        break;

        case ROOM_TROLL_TOLLS_1:            
        break;

        case ROOM_TROLL_TOLLS_2:
        break;

        case ROOM_TROLL_TOLLS_3:

            if(global_troll_tolls_room_3_star) {is_collected = true;}

        break;

        case ROOM_TROLL_TOLLS_4:
        break;

        case ROOM_TROLL_TOLLS_5:

            if(global_troll_tolls_room_5_star) {is_collected = true;}

        break;

        case ROOM_TROLL_TOLLS_6:

            if(global_troll_tolls_room_6_star) {is_collected = true;}

        break;

        case ROOM_TROLL_TOLLS_7:

            if(global_troll_tolls_room_7_star) {is_collected = true;}

        break;

        case ROOM_TROLL_TOLLS_8:
        break;

        case ROOM_TROLL_TOLLS_9:

            if(global_troll_tolls_room_9_star) {is_collected = true;}

        break;

        case ROOM_TROLL_TOLLS_10:
        break;

        case ROOM_TROLL_TOLLS_11:
        break;

        case ROOM_TROLL_TOLLS_12:
        break;

        case ROOM_TROLL_TOLLS_13:
        
            if(global_troll_tolls_room_13_star) {is_collected = true;}

        break;

        case ROOM_TROLL_TOLLS_14:
        break;

        case ROOM_TROLL_TOLLS_15:
        break;

        case ROOM_TROLL_TOLLS_16:
        break;

        default:
        break;
    }

    return is_collected;
}