//#pragma once
//
//#ifdef TWISTED_DEBUG
//	#define EDITOR_BODY() Editor::Editor m_editor;
//
//	#define EDITOR_INIT(app) m_editor.Init(app);
//	#define EDITOR_TERMINATE(app) m_editor.Terminate(app);
//	#define EDITOR_UPDATE(app) m_editor.Update(app);
//#else
//	#define EDITOR_INIT(app)
//	#define EDITOR_TERMINATE(app)
//	#define EDITOR_UPDATE(app)
//#endif

//From file dialog
//#ifdef _WIN32
//#define NOMINMAX
//#include <shellapi.h>
//#include <lmcons.h>
//#pragma comment(lib, "Shell32.lib")
//#else
//#include <unistd.h>
//#include <pwd.h>
//#endif