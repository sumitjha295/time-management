#include <QApplication>
#include <QQmlApplicationEngine>

#include "user.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qmlRegisterType<User>("User", 1, 0, "User");

    //QGuiApplication app(argc, argv);
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/Main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
/*

void SpecialWidget(std::string_view str_id, camera_preview_state& preview_state)
{
    auto scroll_x = ImGui::GetScrollX();
    auto scroll_y = ImGui::GetScrollY();
    ImVec2 margin = ImVec2(20.0, 10.0);
    ImGui::SetCursorPos(ImVec2(scroll_x, scroll_y));
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float height = ImGui::GetWindowHeight();//- ImGui::GetCursorPosY() * 2.0f;
    float width = ImGui::GetWindowWidth();// -  ImGui::GetCursorPosX() * 2.0f;;
    ImGui::InvisibleButton(str_id.data(), ImVec2(width, height));
    ImGui::SetItemUsingMouseWheel();
    const auto is_hovered{ImGui::IsItemHovered()};
    const auto is_dragging{ImGui::IsMouseDragging(ImGuiMouseButton_Left)};


    auto textSize   = ImGui::CalcTextSize(preview_state.text.c_str());
    ImGui::SetCursorPos(ImVec2((width - textSize.x) * 0.5f + scroll_x , scroll_y+margin.y + textSize.y*0.5));
    //ImGui::Text("%d", is_dragging);

    ImGui::Button(preview_state.text.c_str());
    ImGui::SameLine();

    if (!is_hovered) return;
        
    
    if(is_dragging)
    {
        ImGui::SetScrollX(scroll_x- 0.1f * ImGui::GetMouseDragDelta().x);
    }
    else {
        ImGui::SetScrollX(scroll_x-20.0f*ImGui::GetIO().MouseWheelH);
    }
    //draw_list->AddRectFilled(p, ImVec2(p.x + width + scroll_x, p.y + height + scroll_y), IM_COL32(128, 0, 0, 128));
    //draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), IM_COL32(0, 128, 0, 46));

    
    auto long_button_height = 5 * ImGui::GetFrameHeight();
    auto long_button_pos_y = margin.y + (height-long_button_height)*0.5f;
    auto icon_button_size =ImGui::GetFrameHeight();
    auto long_button_width = 0.6f * icon_button_size;
    
    ImGui::SetCursorPos(ImVec2(margin.x + scroll_x, scroll_y +long_button_pos_y));
    
    bool is_pressed = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    bool is_clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    if(IconButton(str_id.data(), close_texture, long_button_width ,long_button_height) && is_pressed){
        ImGui::SetScrollX(scroll_x-speed);
    };
    
    auto left_x = width-2*margin.x +  scroll_x ;
    ImGui::SetCursorPos(ImVec2(left_x, scroll_y + long_button_pos_y));
    if(IconButton(str_id.data(), close_texture, long_button_width ,long_button_height) && is_pressed)
    {
        ImGui::SetScrollX(scroll_x+speed);
    }

    ImGui::SetCursorPos(ImVec2(left_x, margin.y + scroll_y));
    if(IconButton(str_id.data(), close_texture, icon_button_size ,icon_button_size) && is_clicked){
        preview_state.change_mode();
    };

    left_x -= (ImGui::GetItemRectSize().x + 5.0);
    ImGui::SetCursorPos(ImVec2(left_x, margin.y + scroll_y));
    IconButton(str_id.data(), fullscreen_texture, icon_button_size ,icon_button_size);

    
    left_x -= (ImGui::GetItemRectSize().x + 5.0);
    ImGui::SetCursorPos(ImVec2(left_x, margin.y + scroll_y));
    IconButton(str_id.data(), back_texture, icon_button_size ,icon_button_size);
}

// Dear ImGui: standalone example application for OSX + OpenGL2, using legacy fixed pipeline
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#import <Cocoa/Cocoa.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>
#import <algorithm>
#import <vector>
#import <string>
#import <numeric>

#include "imgui.h"
#include "imgui_impl_opengl2.h"
#include "imgui_impl_osx.h"
#include "imgui_internal.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

static GLuint back_texture = 0;
static GLuint close_texture = 0;
static GLuint fullscreen_texture = 0;

const int speed = 4;
int total_images = 3;
const float min_image_view = 0.5;

struct image_resource_data {
    int width;
    int height;
    GLuint texture;
};

struct camera_preview_state
{
    
    std::string text;
    
    inline float get_total_width() const { return total_width;}
    inline float get_visible_width() const { return visible_width;}
    inline float get_left_position() const { return left_position;}
    inline float get_content_ratio() const { return content_ratio;}
    inline bool has_content() const { return ratio > 0.0f;}
    inline bool has_partial_view() const { return parital_view_mode;}
    
    inline void change_position_x(float delta = 0.0f)
    {
        
        left_position += delta;
        left_position = std::min(std::max(left_position, 0.0f), max_left_position);
    }
    
    inline void add_image_data(int width, int height, GLuint texture)
    {
        images.emplace_back(image_resource_data{width, height, texture});
        total_width = std::accumulate(begin(images), end(images), 0.0f ,[](float a, const image_resource_data& b){
            return b.width + a;
        });
        update();
    }

    inline void change_mode()
    {
        parital_view_mode = !parital_view_mode;
        update();

    }
    
    inline float get_ratio() const {return ratio;}
    
    const std::vector<image_resource_data>& get_images() const {return  images;}
    
    void update()
    {
        float min_count = std::min<float>(min_image_view, images.size());
        float max_count = std::max<float>(min_image_view, images.size());
        visible_image_count = parital_view_mode ? min_count: max_count;
        ratio = visible_image_count/max_count;
        visible_width = ratio * total_width;
        max_left_position = total_width - visible_width;
        content_ratio = max_count/visible_image_count;
        change_position_x();
    }
    
private:
    std::vector<image_resource_data> images;
    bool parital_view_mode = false;
    float left_position = 0;
    float visible_image_count  = min_image_view;
    float ratio = 1.0f;
    float total_width = 0.0f;
    float visible_width = 0.0f;
    float max_left_position = 0.0f;
    float content_ratio = 1.0f;

};

camera_preview_state front_camera, back_camera;



// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

bool IconButton(const char* str_id, GLuint texture_id, float button_width, float button_height)
{
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImGui::InvisibleButton(str_id, ImVec2(button_width, button_height));
    
    const bool is_hover = ImGui::IsItemHovered(ImGuiHoveredFlags_RectOnly);
    const bool is_pressed = is_hover && ImGui::IsMouseDown(ImGuiMouseButton_Left);
    float button_radius = 0.5 * button_width;
    
    uint8_t gray_scale = 128 - is_hover * 32 -  is_pressed * 32;
    uint8_t opacity = 128 + is_hover * 32 +  is_pressed * 64;
    ImU32 col_bg =  IM_COL32(gray_scale,gray_scale, gray_scale, opacity);
    
    draw_list->AddRectFilled(p, ImVec2(p.x + button_width, p.y + button_height), col_bg, button_radius);
    
    ImVec2 image_min = ImVec2(p.x, p.y + button_height * 0.5f - button_radius);
    ImVec2 image_max = ImVec2(p.x + button_width, p.y + button_height * 0.5f + button_radius);
    draw_list->AddImage((void*)(intptr_t)texture_id, image_min, image_max);
    
    return is_hover;
}


void ImageDraw(const camera_preview_state& preview_state, float scale)
{
    auto current_left = preview_state.get_left_position();
    auto available_width = preview_state.get_visible_width();
    for(const auto& image: preview_state.get_images())
    {
        if(available_width < 0) break;
        
        auto ratio = float(image.width - current_left)/float(image.width);
        if(available_width < image.width)
        {
            ratio = float(available_width)/float(image.width);
        }
        
        ImVec2 uv0 = ImVec2(current_left/image.width, 0.0f);
        ImVec2 uv1 = ImVec2(current_left/image.width + ratio, 1.0f);
        
        auto image_width = image.width * ratio;
        ImVec2 image_size =ImVec2(image_width * scale, image.height * scale);
        ImGui::Image((void*)(intptr_t)image.texture, image_size, uv0, uv1);
        ImGui::SameLine();
        
        available_width -= (image_width);
        current_left = 0.0f;
    }
}


void SpecialWidget(std::string_view str_id, camera_preview_state& preview_state)
{
    ImVec2 margin = ImVec2(20.0, 10.0);
    ImGui::SetCursorPos(ImVec2(0.0, 0.0));
    //ImVec2 p = ImGui::GetCursorScreenPos();
    
    //ImDrawList* draw_list = ImGui::GetWindowDrawList();
    float height = ImGui::GetWindowHeight() - ImGui::GetCursorPosY() * 2.0f;
    float width = ImGui::GetWindowWidth() -  ImGui::GetCursorPosX() * 2.0f;;
    
    ImGui::InvisibleButton(str_id.data(), ImVec2(width, height));
    const auto is_hovered{ImGui::IsItemHovered()};

    auto textSize   = ImGui::CalcTextSize(preview_state.text.c_str());
    ImGui::SetCursorPos(ImVec2((width - textSize.x) * 0.5f, margin.y + textSize.y*0.5));
    ImGui::Button(preview_state.text.c_str());
    ImGui::SameLine();

    if (!is_hovered) return;
    //draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), IM_COL32(0, 0, 0, 128));
    
    auto long_button_height = 5 * ImGui::GetFrameHeight();
    auto long_button_pos_y = margin.y + (height-long_button_height)*0.5f;
    auto icon_button_size =ImGui::GetFrameHeight();
    auto long_button_width = 0.6f * icon_button_size;
    
    ImGui::SetCursorPos(ImVec2(margin.x, long_button_pos_y));
    
    bool is_pressed = ImGui::IsMouseDown(ImGuiMouseButton_Left);
    bool is_clicked = ImGui::IsMouseClicked(ImGuiMouseButton_Left);
    if(IconButton(str_id.data(), close_texture, long_button_width ,long_button_height) && is_pressed){
        preview_state.change_position_x(-speed);
    };

    ImGui::SetCursorPos(ImVec2(width-margin.x, long_button_pos_y));
    if(IconButton(str_id.data(), close_texture, long_button_width ,long_button_height) && is_pressed)
    {
        preview_state.change_position_x(speed);
    }
    

    
    auto left_x = width-2*margin.x;
    ImGui::SetCursorPos(ImVec2(left_x, margin.y));
    if(IconButton(str_id.data(), close_texture, icon_button_size ,icon_button_size) && is_clicked){
        preview_state.change_mode();
    };

    left_x -= (ImGui::GetItemRectSize().x + 5.0);
    ImGui::SetCursorPos(ImVec2(left_x, margin.y));
    IconButton(str_id.data(), fullscreen_texture, icon_button_size ,icon_button_size);

    
    left_x -= (ImGui::GetItemRectSize().x + 5.0);
    ImGui::SetCursorPos(ImVec2(left_x, margin.y));
    IconButton(str_id.data(), back_texture, icon_button_size ,icon_button_size);
}



-(void)updateAndDrawDemoView
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplOSX_NewFrame(self);
    ImGui::NewFrame();

    // Our state (make them static = more or less global) as a convenience to keep the example terse.
    static bool show_demo_window = true;
    static bool load_image = true;
    static bool show_another_window = true;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        
        //ImGui::Text("front_camera camera = %0.3f", t_anim);

        ImGui::End();
    }

    static GLuint my_image_texture = 0;
    static int my_image_width = 0;
    static int my_image_height = 0;
    
    if(load_image)
    {

        load_image = false;
        bool ret = LoadTextureFromFile("test1.jpeg", &my_image_texture, &my_image_width, &my_image_height);
        IM_ASSERT(ret);
        
        static int back_image_width = 0;
        static int back_image_height = 0;
        ret = LoadTextureFromFile("back.png", &back_texture, &back_image_width, &back_image_height);
        IM_ASSERT(ret);
        
        static int close_image_width = 0;
        static int close_image_height = 0;
        ret = LoadTextureFromFile("close.png", &close_texture, &close_image_width, &close_image_height);
        IM_ASSERT(ret);
        
        
        static int fullscreen_image_width = 0;
        static int fullscreen_image_height = 0;
        ret = LoadTextureFromFile("full_screen.png", &fullscreen_texture, &fullscreen_image_height, &fullscreen_image_width);
        IM_ASSERT(ret);
        
        front_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        //front_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        //front_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        //front_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        //front_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        front_camera.text = "Front";
        
        back_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        //back_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        //back_camera.add_image_data(my_image_width, my_image_height, my_image_texture);
        back_camera.text = "Back";
        back_camera.change_mode();

    }
    // n, ratio, start
    // Arrow buttons with Repeater
    
    // 3. Show another simple window.
    if (show_another_window)
    {
                
        ImGui::Begin("OpenGL Texture Text");
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 0.0f });
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        

        auto window_size = ImGui::GetWindowSize();
        auto window_scale_x = window_size.x * 0.98f/ front_camera.get_total_width();
        bool rear_view_available {back_camera.has_content()  && front_camera.has_partial_view()};
        //ImGuiContext& g = *GImGui;
        //float ANIM_SPEED = 1.0f;
        //float t_anim = ImSaturate(g.LastActiveIdTimer / ANIM_SPEED);
        //float width = float(my_image_width) * (t_anim);
        
        if(front_camera.has_content())
        {
            auto scale = window_scale_x * (rear_view_available ? 0.5f * front_camera.get_content_ratio() : 1.0f);
            bool is_long = my_image_height* scale >  window_size.y/2.0f;

            if(is_long)
            {
                scale = 1.0f;
            }
            
            ImGui::BeginChild("##image_one",
                              ImVec2(front_camera.get_visible_width() * scale, my_image_height* scale));
            ImageDraw(front_camera, scale);
            SpecialWidget(front_camera.text, front_camera);
            ImGui::EndChild();
            
        }
        
        if(rear_view_available)
        {
            ImGui::SameLine();
            auto scale = window_scale_x * 0.5f * front_camera.get_content_ratio();
            bool is_long = my_image_height* scale >  window_size.y/2.0f;
            if(is_long)
            {
                scale = 1.0f;
            }
            ImGui::BeginChild("##image_two",
                              ImVec2(back_camera.get_visible_width()* scale, my_image_height* scale));
            ImageDraw(back_camera, scale);
            SpecialWidget(back_camera.text, back_camera);
            ImGui::EndChild();
        }
        ImGui::PopStyleVar(2);
        ImGui::Text("get_visible_width = %0.3f", front_camera.get_visible_width());
        ImGui::Text("get_total_width = %0.3f", front_camera.get_total_width());
        ImGui::Text("get_ratio = %0.3f", front_camera.get_ratio());
        ImGui::Text("get left position = %0.3f", front_camera.get_left_position());
        ImGui::Text("total_images= %d",  total_images );

        ImGui::End();
    }
    



    // Rendering
    ImGui::Render();
   
    ImDrawData* draw_data = ImGui::GetDrawData();

    [[self openGLContext] makeCurrentContext];
    GLsizei width  = (GLsizei)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    GLsizei height = (GLsizei)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    glViewport(0, 0, width, height);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL2_RenderDrawData(draw_data);

    // Present
    [[self openGLContext] flushBuffer];

    if (!animationTimer)
        animationTimer = [NSTimer scheduledTimerWithTimeInterval:0.017 target:self selector:@selector(animationTimerFired:) userInfo:nil repeats:YES];
}


*/
