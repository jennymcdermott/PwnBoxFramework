#pragma once

/*
* PwnBoxFramework is a library for windows
* designed by LordKauck to make game hacks
* easier to produce by providing the tools necesssary
* for those wanting to use it to hack games.
* 
* Note: To specify if you are making an internal hack or not,
*		set PWNBOX_BUILD_DLL to 1 in Base.h
* 
* Note: 
*		If you are making an external hack, it is recommended that 
*		you use the renderer for making a GUI with your hack!!
*		It uses DirectX11 though there may be support for Vulkan or
*		older versions of DirectX on the way.
* 
* ---------------------------------------------------------
* To get started using PwnBoxFramework:
* ---------------------------------------------------------
* - Create a new project
* - Add PwnBoxFramework as a reference
* - Set it up similarly to how PwnBoxFramework is set up (i.e. C++ Settings, include dirs,etc.)
* - Create a new class that has no methods in your project, within the namespace of PwnBoxFramework
* - Include "PwnBoxFramework.h" into your project where you created the class
* - Include "PwnBoxFramework/Base/Entrypoint.h" into your project where you created the class
* - Set the class that you have created as a public child of Application
* - Inside the constructor of your class, put PushLayer(new *INSERT YOUR LAYER HERE*);
* - Inside the cpp file where your class is, define CreateApp() 
* -		as Application* CreateApp() noexcept { return new *YOURCLASS*(); }
* - Create a new layer (see Layer.h for virtual functions)
*/

//--------------- Base App Files ------------------
#include "PwnBoxFramework/Base/Application.h"
#include "PwnBoxFramework/Layer/Layer.h"
#include "PwnBoxFramework/Layer/LayerStack.h"
//-------------------------------------------------

//-------------- Memory Class Files ----------------
#include "PwnBoxFramework/Memory/Memory.h"
#include "PwnBoxFramework/Memory/PatternScanner.h"
#include "PwnBoxFramework/Memory/Injector.h"
//--------------------------------------------------

//----------- Mathematics Files ---------------
#include "PwnBoxFramework/Math/Vector.h"
//---------------------------------------------

//==------------- Window/Renderer Class Files ----------------
#include "PwnBoxFramework/Window/AspectInfo.h"
#include "PwnBoxFramework/Window/WindowProps.h"
#include "PwnBoxFramework/Renderer/Renderer.h"
#include "PwnBoxFramework/Window/Window.h"
//------------------------------------------------------------