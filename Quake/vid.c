// short circuited minimum linkage for some globals
#include "quakedef.h"
#include "cfgfile.h"
#include "vid.h"

// from gl_vidsdl.c
viddef_t	vid;				// global video state
modestate_t	modestate = MS_UNINIT;
qboolean	scr_skipupdate;

qboolean gl_mtexable = false;
qboolean gl_texture_env_combine = false; //johnfitz
qboolean gl_texture_env_add = false; //johnfitz
qboolean gl_swap_control = false; //johnfitz
qboolean gl_anisotropy_able = false; //johnfitz
float gl_max_anisotropy; //johnfitz
qboolean gl_texture_NPOT = false; //ericw
qboolean gl_vbo_able = false; //ericw
qboolean gl_glsl_able = false; //ericw
int32_t gl_max_texture_units = 0; //ericw
qboolean gl_glsl_gamma_able = false; //ericw
qboolean gl_glsl_alias_able = false; //ericw
int gl_stencilbits;

//johnfitz -- new cvars
static cvar_t	vid_fullscreen = {"vid_fullscreen", "0", CVAR_ARCHIVE};	// QuakeSpasm, was "1"
static cvar_t	vid_width = {"vid_width", "800", CVAR_ARCHIVE};		// QuakeSpasm, was 640
static cvar_t	vid_height = {"vid_height", "600", CVAR_ARCHIVE};	// QuakeSpasm, was 480
static cvar_t	vid_bpp = {"vid_bpp", "16", CVAR_ARCHIVE};
static cvar_t	vid_refreshrate = {"vid_refreshrate", "60", CVAR_ARCHIVE};
static cvar_t	vid_vsync = {"vid_vsync", "0", CVAR_ARCHIVE};
static cvar_t	vid_fsaa = {"vid_fsaa", "0", CVAR_ARCHIVE}; // QuakeSpasm
static cvar_t	vid_desktopfullscreen = {"vid_desktopfullscreen", "0", CVAR_ARCHIVE}; // QuakeSpasm
static cvar_t	vid_borderless = {"vid_borderless", "0", CVAR_ARCHIVE}; // QuakeSpasm
//johnfitz
cvar_t		vid_gamma = {"gamma", "1", CVAR_ARCHIVE}; //johnfitz -- moved here from view.c
cvar_t		vid_contrast = {"contrast", "1", CVAR_ARCHIVE}; //QuakeSpasm, MarkV

void VID_Init (void)
{
	const char	*read_vars[] = {
           "vid_fullscreen",
					 "vid_width",
					 "vid_height",
					 "vid_refreshrate",
					 "vid_bpp",
					 "vid_vsync",
					 "vid_fsaa",
					 "vid_desktopfullscreen",
					 "vid_borderless"};
#define num_readvars	( sizeof(read_vars)/sizeof(read_vars[0]) )

	Cvar_RegisterVariable (&vid_fullscreen); //johnfitz
	Cvar_RegisterVariable (&vid_width); //johnfitz
	Cvar_RegisterVariable (&vid_height); //johnfitz
	Cvar_RegisterVariable (&vid_refreshrate); //johnfitz
	Cvar_RegisterVariable (&vid_bpp); //johnfitz
	Cvar_RegisterVariable (&vid_vsync); //johnfitz
	Cvar_RegisterVariable (&vid_fsaa); //QuakeSpasm
	Cvar_RegisterVariable (&vid_desktopfullscreen); //QuakeSpasm
	Cvar_RegisterVariable (&vid_borderless); //QuakeSpasm
	// Cvar_SetCallback (&vid_fullscreen, VID_Changed_f);
	// Cvar_SetCallback (&vid_width, VID_Changed_f);
	// Cvar_SetCallback (&vid_height, VID_Changed_f);
	// Cvar_SetCallback (&vid_refreshrate, VID_Changed_f);
	// Cvar_SetCallback (&vid_bpp, VID_Changed_f);
	// Cvar_SetCallback (&vid_vsync, VID_Changed_f);
	// Cvar_SetCallback (&vid_fsaa, VID_FSAA_f);
	// Cvar_SetCallback (&vid_desktopfullscreen, VID_Changed_f);
	// Cvar_SetCallback (&vid_borderless, VID_Changed_f);
	
	// Cmd_AddCommand ("vid_unlock", VID_Unlock); //johnfitz
	// Cmd_AddCommand ("vid_restart", VID_Restart); //johnfitz
	// Cmd_AddCommand ("vid_test", VID_Test); //johnfitz
	// Cmd_AddCommand ("vid_describecurrentmode", VID_DescribeCurrentMode_f);
	// Cmd_AddCommand ("vid_describemodes", VID_DescribeModes_f);

  vid.width  = 1024;
  vid.height = 576;
	Cvar_SetValueQuick (&vid_bpp, (float)8);

	if (CFG_OpenConfig("config.cfg") == 0)
	{
		CFG_ReadCvars(read_vars, num_readvars);
		CFG_CloseConfig();
	}
	CFG_ReadCvarOverrides(read_vars, num_readvars);

	vid.maxwarpwidth  = 320;
	vid.maxwarpheight = 200;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));
  // ??
	// vid_menucmdfn = VID_Menu_f; //johnfitz
	// vid_menudrawfn = VID_MenuDraw;
	// vid_menukeyfn = VID_MenuKey;
}
void VID_Shutdown (void) {}
void VID_Update (vrect_t *rects) {}
void VID_SyncCvars (void) {}
void VID_Toggle (void) {}
void *VID_GetWindow (void) { return 0; }
qboolean VID_HasMouseOrInputFocus (void) { return true; }
qboolean VID_IsMinimized (void) { return false; }
void	VID_Lock (void) {}
