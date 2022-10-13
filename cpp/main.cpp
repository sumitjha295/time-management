// Dear ImGui: standalone example application for OSX + Metal.
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

#import <Foundation/Foundation.h>

#if TARGET_OS_OSX
#import <Cocoa/Cocoa.h>
#else
#import <UIKit/UIKit.h>
#endif

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_metal.h"
#if TARGET_OS_OSX
#include "imgui_impl_osx.h"
@interface AppViewController : NSViewController<NSWindowDelegate>
@end
#else
@interface AppViewController : UIViewController
@end
#endif

@interface AppViewController () <MTKViewDelegate>
@property (nonatomic, readonly) MTKView *mtkView;
@property (nonatomic, strong) id <MTLDevice> device;
@property (nonatomic, strong) id <MTLCommandQueue> commandQueue;
@end

//-----------------------------------------------------------------------------------
// AppViewController
//-----------------------------------------------------------------------------------

@implementation AppViewController

-(instancetype)initWithNibName:(nullable NSString *)nibNameOrNil bundle:(nullable NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];

    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];

    if (!self.device)
    {
        NSLog(@"Metal is not supported");
        abort();
    }

    // Setup Dear ImGui context
    // FIXME: This example doesn't have proper cleanup...
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Renderer backend
    ImGui_ImplMetal_Init(_device);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("/Users/sumit/Projects/imgui/misc/fonts/Roboto-Medium.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("/Users/sumit/Projects/imgui/misc/fonts/Cousine-Regular.ttf", 15.0f);
    // io.Fonts->AddFontFromFileTTF("/Users/sumit/Projects/imgui/misc/fonts/DroidSans.ttf", 16.0f);
    // io.Fonts->AddFontFromFileTTF("/Users/sumit/Projects/imgui/misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    return self;
}

-(MTKView *)mtkView
{
    return (MTKView *)self.view;
}

-(void)loadView
{
    self.view = [[MTKView alloc] initWithFrame:CGRectMake(0, 0, 1200, 720)];
}

-(void)viewDidLoad
{
    [super viewDidLoad];

    self.mtkView.device = self.device;
    self.mtkView.delegate = self;

#if TARGET_OS_OSX
    ImGui_ImplOSX_Init(self.view);
    [NSApp activateIgnoringOtherApps:YES];
#endif
}

-(void)drawInMTKView:(MTKView*)view
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = view.bounds.size.width;
    io.DisplaySize.y = view.bounds.size.height;
    static auto font_ptr1 = io.Fonts->AddFontFromFileTTF("/Users/sumit/Projects/imgui/misc/fonts/Roboto-Medium.ttf", 16.0f);
    static auto font_ptr = io.Fonts->AddFontFromFileTTF("/Users/sumit/Projects/imgui/misc/fonts/Roboto-Medium.ttf", 10.0f);


#if TARGET_OS_OSX
    CGFloat framebufferScale = view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;
#else
    CGFloat framebufferScale = view.window.screen.scale ?: UIScreen.mainScreen.scale;
#endif
    io.DisplayFramebufferScale = ImVec2(framebufferScale, framebufferScale);

    id<MTLCommandBuffer> commandBuffer = [self.commandQueue commandBuffer];

    MTLRenderPassDescriptor* renderPassDescriptor = view.currentRenderPassDescriptor;
    if (renderPassDescriptor == nil)
    {
        [commandBuffer commit];
		return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplMetal_NewFrame(renderPassDescriptor);
#if TARGET_OS_OSX
    ImGui_ImplOSX_NewFrame(view);
#endif
    ImGui::NewFrame();

    // Our state (make them static = more or less global) as a convenience to keep the example terse.
    static bool show_demo_window = false;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;


    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", nullptr, window_flags);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar(2);
    // DockSpace
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        static auto first_time = true;

        if (first_time)
        {
            first_time = false;

            ImGui::DockBuilderRemoveNode(dockspace_id); // clear any previous layout
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            // split the dockspace into 2 nodes -- DockBuilderSplitNode takes in the following args in the following order
            //   window ID to split, direction, fraction (between 0 and 1), the final two setting let's us choose which id we want (which ever one we DON'T set as NULL, will be returned by the function)
            //                                                              out_id_at_dir is the id of the node in the direction we specified earlier, out_id_at_opposite_dir is in the opposite direction
            auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.5f, nullptr, &dockspace_id);
            auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.5f, nullptr, &dockspace_id);

            // we now dock our windows into the docking node we made above
            ImGui::DockBuilderDockWindow("Down", dock_id_down);
            ImGui::DockBuilderDockWindow("Left", dock_id_left);
            ImGui::DockBuilderFinish(dockspace_id);
        }
    }

    ImGui::End();
    static float padding_x = 62.0f;
    static float padding_y = 10.0f;
    static float rad = 3.0f;
    
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;
    static float menu_width = 120.0f;
    static float button_width = 32.0f;

    ImGui::SetNextWindowClass(&window_class);
    static bool show_notification = true;
    static bool force_notification = true;

    if (ImGui::Begin("Down", nullptr, ImGuiWindowFlags_MenuBar|ImGuiWindowFlags_NoTitleBar)) {
      if (ImGui::BeginMenuBar()) {
        ImGui::Text("%s", "dffdfsgsdgdsgsdgs");
        ImGui::EndMenuBar();
      }
        ImGui::SliderFloat("menu_width", &menu_width, 10.0f, 100.0f);
        ImGui::SliderFloat("button_width", &button_width, 10.0f, 200.0f);
        ImGui::SliderFloat("padding_x", &padding_x, 1.0f, 100.0f);
        ImGui::SliderFloat("padding_y", &padding_y, 1.0f, 10.0f);
        ImGui::SliderFloat("rad", &rad, 1.0f, 10.0f);
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("show_notification", &show_notification);
        if(ImGui::Button("force_notification"))
        {
            force_notification = true;
        }


    ImGui::End();
    }
   


    static bool show_demo_hello = true;
   
    auto draw_button = [&](auto id, auto text, auto on_click, auto on_hover){
        ImTextureID my_tex_id = io.Fonts->TexID;
        float my_tex_w = (float)io.Fonts->TexWidth;
        float my_tex_h = (float)io.Fonts->TexHeight;

        ImVec2 size = ImVec2(32.0f, 32.0f);                     // Size of the image we want to make visible
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);                        // UV coordinates for lower-left
        ImVec2 uv1 = ImVec2(32.0f / my_tex_w, 32.0f / my_tex_h);// UV coordinates for (32,32) in our texture
        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);         // Black background
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);       // No tint



        //ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);
        ImGui::PushID(id);
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const ImVec2 p1 = ImGui::GetCursorScreenPos();
        auto click = ImGui::ImageButton(my_tex_id, size, uv0, uv1, 0, bg_col, tint_col);
        draw_list->AddCircleFilled(ImVec2(p1.x + size.x-5.0f, p1.y + 8.0f), rad, ImGui::GetColorU32(ImVec4(1.0f, .0f, .0f, 1.0f)));
        ImGui::PopID();
        if(click)
        {
            ImGui::OpenPopup(on_click);
        }
        else if((ImGui::IsItemHovered() && show_notification) || force_notification)
        {
            ImGui::OpenPopup(on_hover);
        }
       // ImGui::PopStyleVar();
    };


   
    ImGui::SetNextWindowClass(&window_class);
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, .0f, 1.0f, 1.0f)); // Menu bar background color
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(padding_x,padding_y));
    ImGuiWindowFlags window_flags_1 = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;

    ImGui::Begin("Left", &show_demo_hello, window_flags_1);
    //ImGui::PushClipRect(clip_rect.Min, clip_rect.Max, true);

    if (ImGui::BeginMenuBar())
    {
        // ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,ImVec2(0,0));

        ImGui::Text("This is header");
        {
            ImGui::SetCursorPos({ImGui::GetWindowWidth()-button_width, ImGui::GetScrollY()});
            auto pos = ImGui::GetCursorScreenPos();
           
            draw_button("a","+", "on_click", "on_hover");
            
            ImGui::SetNextWindowPos({pos.x+ button_width-menu_width, ImGui::GetCursorScreenPos().y + ImGui::GetItemRectSize().y + padding_y});
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f)); // Menu bar background color
            ImGui::SetNextWindowSize({menu_width,0.0f});
            if (ImGui::BeginPopup("on_click"))
            {
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{2.0f, padding_y});
                ImGui::PushFont(font_ptr);
                ImGui::Text("Sumit Jha");
                ImGui::PopFont();
                ImGui::Text("sja@adfasd");
                ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.0f, .0f, 0.0f, 1.0f)); // Menu bar background color
                ImGui::Separator();
                ImGui::PopStyleColor(1);
                ImGui::MenuItem("Reauthenticate");
                ImGui::PopStyleVar();
                ImGui::EndPopup();
     
            }
            ImGui::PopStyleColor(1);
            
            ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, 5.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 0.0f);
            ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f)); // Menu bar background color
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(.0f, .0f, .0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(.0f, .0f, .0f, 0.0f));
            
            ImGui::SetNextWindowPos({pos.x+ button_width-2.5f*menu_width, ImGui::GetCursorScreenPos().y + ImGui::GetItemRectSize().y + padding_y});
            ImGui::SetNextWindowSize({2.5f*menu_width,0.0f});
            if (ImGui::BeginPopup("on_hover"))
            {
                ImGui::PushTextWrapPos();
                ImGui::Text("You access them through the imgui.get_io() function: import imgui io = imgui.get_io(). Once you have access to the font atlas you can load custom fonts");
                ImGui::PopTextWrapPos();
                ImGui::Dummy({padding_x, 0.0f});
                ImGui::SameLine();
                ImGui::Button("Reauthenticate");
                
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                auto l = 2.0f*padding_y;
                auto p1 = ImVec2{ImGui::GetWindowPos().x +  ImGui::GetWindowSize().x - 2.0f, ImGui::GetWindowPos().y + 2.0f};
                auto p2 = ImVec2{p1.x - l, p1.y};
                auto p3 = ImVec2{0.5f*(p1.x + p2.x),  p2.y - l*0.5f};
 
                show_notification = false;
                force_notification = false;
                ImGui::EndPopup();
                //draw_list->AddCircleFilled(p1, 20.0f,  IM_COL32(255, 0, 0, 255));
                draw_list->AddTriangleFilled(p1, p2, p3, IM_COL32(255, 255, 255, 255));
            }
            ImGui::PopStyleColor(3);
            ImGui::PopStyleVar(2);

        }
        //ImGui::PopStyleVar(1);

        ImGui::EndMenuBar();
        //ImGui::PopClipRect();
        
     }

    ImGui::End();
    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(1);


    
    
    
    /*

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        static bool show_demo_hello = true;
        
        

        ImGui::Begin("Another Window", &show_another_window, ImGuiWindowFlags_NoTitleBar);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
        

        ImGui::Begin("Menu bar", &show_demo_hello, ImGuiWindowFlags_MenuBar|ImGuiWindowFlags_NoTitleBar);
        auto width = ImGui::GetWindowWidth();
        if (ImGui::BeginMenuBar()) {
             
            ImGui::Text("Happy secondary menu bar");
            {
                ImGui::SameLine();
                auto pos = ImGui::GetCursorScreenPos();
                
                ImGui::SetCursorPos({width-20, ImGui::GetScrollY()});
                pos = ImGui::GetCursorScreenPos();
                if (ImGui::Button("-")) ImGui::OpenPopup("tstElementList2");
               
                ImGui::SetNextWindowPos({pos.x-82, ImGui::GetCursorScreenPos().y+20});
                if (ImGui::BeginPopup("tstElementList2"))
                {
                   
                    ImGui::MenuItem("element 1");
                    ImGui::MenuItem("element 2");
                    auto p = ImGui::GetItemRectSize().x *0.9f;
                    ImGui::Button("Yes", ImVec2(p, 0.0f));
                    ImGui::Button("No", ImVec2(p, 0.0f));
                    ImGui::EndPopup();
                }
            }
            {
                ImGui::SameLine();
                auto pos = ImGui::GetCursorScreenPos();
                
                ImGui::SetCursorPos({width-40, ImGui::GetScrollY()});
                pos = ImGui::GetCursorScreenPos();
                if (ImGui::Button("+")) ImGui::OpenPopup("tstElementList3");
               
                ImGui::SetNextWindowPos({pos.x-102, ImGui::GetCursorScreenPos().y+20});
                if (ImGui::BeginPopup("tstElementList3"))
                {
                   
                    ImGui::MenuItem("element 11");
                    ImGui::MenuItem("element 22");
                    auto p = ImGui::GetItemRectSize().x *0.9f;
                    ImGui::Button("Yes", ImVec2(p, 0.0f));
                    ImGui::Button("No", ImVec2(p, 0.0f));
                    ImGui::EndPopup();
                }
            }
             ImGui::EndMenuBar();
         }
 
        ImGui::End();
        

        ImGui::Begin("Hello, world!", &show_demo_hello, ImGuiWindowFlags_NoTitleBar);


        // Create a window called "Hello, world!" and append into it.
        
        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }
    */
    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();

    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    [renderEncoder pushDebugGroup:@"Dear ImGui rendering"];
    ImGui_ImplMetal_RenderDrawData(draw_data, commandBuffer, renderEncoder);
    [renderEncoder popDebugGroup];
    [renderEncoder endEncoding];

	// Present
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

-(void)mtkView:(MTKView*)view drawableSizeWillChange:(CGSize)size
{
}

//-----------------------------------------------------------------------------------
// Input processing
//-----------------------------------------------------------------------------------

#if TARGET_OS_OSX

- (void)viewWillAppear
{
    [super viewWillAppear];
    self.view.window.delegate = self;
}

- (void)windowWillClose:(NSNotification *)notification
{
    ImGui_ImplMetal_Shutdown();
    ImGui_ImplOSX_Shutdown();
    ImGui::DestroyContext();
}

#else

// This touch mapping is super cheesy/hacky. We treat any touch on the screen
// as if it were a depressed left mouse button, and we don't bother handling
// multitouch correctly at all. This causes the "cursor" to behave very erratically
// when there are multiple active touches. But for demo purposes, single-touch
// interaction actually works surprisingly well.
-(void)updateIOWithTouchEvent:(UIEvent *)event
{
    UITouch *anyTouch = event.allTouches.anyObject;
    CGPoint touchLocation = [anyTouch locationInView:self.view];
    ImGuiIO &io = ImGui::GetIO();
    io.AddMousePosEvent(touchLocation.x, touchLocation.y);

    BOOL hasActiveTouch = NO;
    for (UITouch *touch in event.allTouches)
    {
        if (touch.phase != UITouchPhaseEnded && touch.phase != UITouchPhaseCancelled)
        {
            hasActiveTouch = YES;
            break;
        }
    }
    io.AddMouseButtonEvent(0, hasActiveTouch);
}

-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event      { [self updateIOWithTouchEvent:event]; }
-(void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event      { [self updateIOWithTouchEvent:event]; }
-(void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event  { [self updateIOWithTouchEvent:event]; }
-(void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event      { [self updateIOWithTouchEvent:event]; }

#endif

@end

//-----------------------------------------------------------------------------------
// AppDelegate
//-----------------------------------------------------------------------------------

#if TARGET_OS_OSX

@interface AppDelegate : NSObject <NSApplicationDelegate>
@property (nonatomic, strong) NSWindow *window;
@end

@implementation AppDelegate

-(BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)sender
{
    return YES;
}

-(instancetype)init
{
    if (self = [super init])
    {
        NSViewController *rootViewController = [[AppViewController alloc] initWithNibName:nil bundle:nil];
        self.window = [[NSWindow alloc] initWithContentRect:NSZeroRect
                                                  styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable
                                                    backing:NSBackingStoreBuffered
                                                      defer:NO];
        self.window.contentViewController = rootViewController;
        [self.window center];
        [self.window makeKeyAndOrderFront:self];
    }
    return self;
}

@end

#else

@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@end

@implementation AppDelegate

-(BOOL)application:(UIApplication *)application
    didFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions
{
    UIViewController *rootViewController = [[AppViewController alloc] init];
    self.window = [[UIWindow alloc] initWithFrame:UIScreen.mainScreen.bounds];
    self.window.rootViewController = rootViewController;
    [self.window makeKeyAndVisible];
    return YES;
}

@end

#endif

//-----------------------------------------------------------------------------------
// Application main() function
//-----------------------------------------------------------------------------------

#if TARGET_OS_OSX

int main(int argc, const char * argv[])
{
    return NSApplicationMain(argc, argv);
}

#else

int main(int argc, char * argv[])
{
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
}

#endif
