cmake_minimum_required(VERSION 3.27)
project(2025_I2P2_Final_Project)

set(CMAKE_CXX_STANDARD 17)
set(ALLEGRO_VSYNC 2) # Turn off vsync

include_directories(.)
include_directories(Resource)

file(COPY Resource/audios DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/fonts DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource/images DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY Resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY allegro_monolith-5.2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(2025_I2P2_Final_Project
    Bullet/Bullet.cpp
    Bullet/Bullet.hpp
    Bullet/FireBullet.cpp
    Bullet/FireBullet.hpp

    Engine/Allegro5Exception.cpp
    Engine/Allegro5Exception.hpp
    Engine/AudioHelper.cpp
    Engine/AudioHelper.hpp
    Engine/Collider.cpp
    Engine/Collider.hpp
    Engine/GameEngine.cpp
    Engine/GameEngine.hpp
    Engine/Group.cpp
    Engine/Group.hpp
    Engine/IControl.cpp
    Engine/IControl.hpp
    Engine/IObject.cpp
    Engine/IObject.hpp
    Engine/IScene.cpp
    Engine/IScene.hpp
    Engine/LOG.cpp
    Engine/LOG.hpp
    Engine/Point.cpp
    Engine/Point.hpp
    Engine/Resources.cpp
    Engine/Resources.hpp
    Engine/Sprite.cpp
    Engine/Sprite.hpp

    Note/Note.cpp
    Note/Note.hpp
    Note/RedNote.cpp
    Note/RedNote.hpp
    Note/BlueNote.cpp
    Note/BlueNote.hpp

    Player/Player.cpp
    Player/Player.hpp
    
    Scene/LoseScene.cpp
    Scene/LoseScene.hpp
    Scene/PlayScene.cpp
    Scene/PlayScene.hpp
    Scene/StageSelectScene.cpp
    Scene/StageSelectScene.hpp
    Scene/StartScene.cpp
    Scene/StartScene.h
    Scene/WinScene.cpp
    Scene/WinScene.hpp
    Scene/SettingsScene.cpp
    Scene/SettingsScene.hpp
    Scene/LoginScene.cpp
    Scene/LoginScene.hpp
    Scene/RegisterScene.cpp
    Scene/RegisterScene.hpp

    UI/Animation/MissEffect.cpp
    UI/Animation/MissEffect.hpp
    UI/Animation/GreatEffect.cpp
    UI/Animation/GreatEffect.hpp
    UI/Animation/PerfectEffect.cpp
    UI/Animation/PerfectEffect.hpp
    
    UI/Component/Image.cpp
    UI/Component/Image.hpp
    UI/Component/ImageButton.cpp
    UI/Component/ImageButton.hpp
    UI/Component/Label.cpp
    UI/Component/Label.hpp
    UI/Component/Slider.cpp
    UI/Component/Slider.hpp
    
    main.cpp
)

IF (WIN32)
    target_include_directories(2025_I2P2_Final_Project PUBLIC C:/allegro/include)
    target_link_directories(2025_I2P2_Final_Project PUBLIC C:/allegro/lib)
    target_compile_options(2025_I2P2_Final_Project PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_Final_Project liballegro_monolith)
ELSE()
    find_package(PkgConfig)
    pkg_check_modules(ALLEGRO5 REQUIRED allegro-5 allegro_main-5 allegro_font-5 allegro_dialog-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)
    target_include_directories(2025_I2P2_Final_Project PUBLIC lib)
    target_include_directories(2025_I2P2_Final_Project PUBLIC ${ALLEGRO5_INCLUDE_DIRS})
    target_compile_options(2025_I2P2_Final_Project PUBLIC ${ALLEGRO5_CFLAGS_OTHER})
    target_link_libraries(2025_I2P2_Final_Project ${ALLEGRO5_LIBRARIES})
    target_link_directories(2025_I2P2_Final_Project PUBLIC ${ALLEGRO5_LIBRARY_DIRS})
ENDIF()