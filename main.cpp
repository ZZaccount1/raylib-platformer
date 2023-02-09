#include <iostream>
#include <raylib.h>
#include <fstream>
#include <vector>

const int screenWidth = 800;
const int screenHeight = 450;

struct Player {
    Vector2 position;
    float jumpSpeed;
    float fallSpeed;
    int size;
};

int main() 
{
    // Initialize window
    InitWindow(screenWidth, screenHeight, "Basic Platformer");
    SetTargetFPS(60);

    // Create player
    Player player = { { 300.0f, 100.0f }, 0.0f, 0.0f, 20};

    // Create platform
    Rectangle platform = { screenWidth / 2 - 50.0f, screenHeight / 2 - 25.0f, 100.0f, 50.0f };

    // Define camera
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2, screenHeight/2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // tile map reading
    std::string tilemapPath = "main.map";
    std::ifstream map(tilemapPath);

    int tileSize = 64;

    std::string line;

    std::vector<std::vector<int>> tilemap;   
    int num_rows = 0, num_cols = 0;

    while(getline(map, line))
    {
        num_rows++;
        int col = 0;
        std::string num = "";
        for(int i = 0; i<line.length(); i++)
        {
            if(line[i] != ' ')
            {
                num += line[i];
            }
            else
            {
                col++;
                num = "";
            }
        }
        col++;
        if(num_cols == 0)
        {
            num_cols = col;
        }
    }
    map.close();

    std::cout << num_rows << " and " << num_cols << std::endl;

    tilemap.resize(num_rows, std::vector<int>(num_cols));

    map.open(tilemapPath);
    int row=0;
    while(getline(map, line))
    {
        int col = 0;
        std::string num = "";
        for(int i =0;i<line.length(); i++)
        {
            if(line[i] != ' ')
            {
                num += line[i];
            }
            else
            {
                tilemap[row][col] = std::stoi(num);
                num = "";
                col++;
            }
        }
        tilemap[row][col] = std::stoi(num);
        row++;
    }
    map.close();

    //debug
    for(int i = 0; i < num_rows; i++)
    {
        for(int o= 0;o < num_cols; o++)
        {
            std::cout << tilemap[i][o] << " ";
        }
        std::cout<<std::endl;
    }

    bool playerGrounded = false;
    bool playerGroundedSwap = false;

    // Main game loop
    while (!WindowShouldClose()) 
    {
        // Input handling
        if (IsKeyDown(KEY_RIGHT)) player.position.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) player.position.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) player.position.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) player.position.y += 2.0f;
        if (IsKeyPressed(KEY_SPACE) && (playerGrounded || playerGrounded)) 
        {
            player.jumpSpeed = -8.0f;      
            playerGrounded = false;
            playerGroundedSwap = false;
        }

        // Apply gravity
        player.fallSpeed += 0.2f;
        player.position.y += player.jumpSpeed + player.fallSpeed;

        Rectangle playerRec = { player.position.x, player.position.y, player.size, player.size};

        for(int i = 0; i < num_rows; i++)
        {
            for(int o = 0; o < num_cols; o++)
            {
                if(tilemap[i][o]==1)
                {
                    Rectangle platform = {(float)o*tileSize, (float)i*tileSize, tileSize, tileSize};

                    // Collision detection with platform
                    if (CheckCollisionRecs(playerRec, platform))
                    {
                        float playerLeft = player.position.x;
                        float playerRight = player.position.x + player.size;
                        float playerTop = player.position.y;
                        float playerBottom = player.position.y + player.size;
                        float platformLeft = platform.x;
                        float platformRight = platform.x + platform.width;
                        float platformTop = platform.y;
                        float platformBottom = platform.y + platform.height;

                        float overlapLeft = playerRight - platformLeft;
                        float overlapRight = platformRight - playerLeft;
                        float overlapTop = platformBottom - playerTop;
                        float overlapBottom = playerBottom - platformTop;

                        if (overlapLeft < overlapRight && overlapLeft < overlapTop && overlapLeft < overlapBottom) 
                        {
                            player.position.x -= overlapLeft;
                            std::cout<<"col 1"<<std::endl;
                        } 
                        else if (overlapRight < overlapTop && overlapRight < overlapBottom) 
                        {
                            player.position.x += overlapRight;
                            std::cout<<"col 2"<<std::endl;
                        }
                        else if (overlapTop < overlapBottom) {
                            player.position.y = platform.y + platform.height;
                            player.jumpSpeed = 0.0f;
                            player.fallSpeed = 0.0f;
                            playerGrounded = true;
                            playerGroundedSwap = true;
                            std::cout<<"col 3"<<std::endl;
                        } 
                        else
                        {
                            player.position.y = platform.y - player.size;
                            player.jumpSpeed = 0.0f;
                            player.fallSpeed = 0.0f;
                            playerGrounded = true;
                            playerGroundedSwap = true;
                            std::cout<<"col 4"<<std::endl;
                        }
                    } 
                    else
                    {
                        playerGrounded = false;
                        std::cout<<"col false"<<std::endl;
                    }
                }
            }
        }
        playerGrounded = playerGroundedSwap;

        std::cout<<playerGrounded<< " " << playerGroundedSwap<<std::endl;

        playerGroundedSwap = false;

        camera.target = player.position;

        // Clear screen
        BeginDrawing();
            BeginMode2D(camera);

            ClearBackground(RAYWHITE);

            // Draw player
            DrawRectangle(player.position.x, player.position.y, player.size, player.size, RED);

            for(int i = 0; i < num_rows; i++)
            {
                for(int o = 0; o < num_cols; o++)
                {
                    if(tilemap[i][o]==1)
                    {
                        DrawRectangleV({(float)o*tileSize, (float)i*tileSize}, {(float)tileSize, (float)tileSize}, BLACK);
                    }
                }
            }

        EndDrawing();
    }

    // Close window and unload all resources
    CloseWindow();

    return 0;
}
