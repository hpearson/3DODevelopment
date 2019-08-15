/**************************************************************************
 * TRSI Anniversary Demo                                                  *
 **************************************************************************
 *                                                                        *
 * Platform : 3DO Console                                                 *
 *                                                                        *
 * Party: 2015 Revision                                                   *
 *                                                                        *
 **************************************************************************/

#include "types.h"
#include "main.h"

#include "tools.h"

#include "engine_main.h"

#include "fx_plasma.h"
#include "fx_sphere.h"
#include "fx_blobs.h"
#include "fx_rotblobs.h"
#include "fx_stars.h"
#include "fx_sphere.h"
#include "fx_fire.h"
#include "fx_bump.h"
#include "fx_polar.h"
#include "fx_clouds.h"
#include "fx_test.h"
#include "fx_linetests.h"
#include "fx_dots.h"

#include "mathutil.h"

bool vsync = true;

/*** globals ***/

/* An IOReq for SPORT operations */
Item    gVRAMIOReq;
Item    vsyncItem;

int32 Debug_flag=NO;
int32 Rotate_object=NO;
int32 Number_of_loops=15;
int32 View_X=0, View_Y=0, Camera_angle=0;
int32 Delta_x=0, Delta_y=0, Delta_camera_angle;

Item BitmapItems[2];
Bitmap *Bitmaps[2];
ubyte *BackPic = NULL;

ScreenContext Screencontext;
int32 Camera_id=0;
int32 World_id=0;
int32 Rotate_stuff=0;

char *Celfile1="3d_data/sw.cel";
char *Celfile2="3d_data/alt.cel";
char *Backdrop_file="3d_data/backdrop.3do";

CCB *Ccbs[5];                             /* ccb */

MemInfo *mi0, *mi1;

int buttB = 0;
int buttC = 0;
int buttUp = 0;
int buttDown = 0;

/***********************************************************************
 * close_everything :                                                  *
 ***********************************************************************/
void
close_everything( void )
{
   ENTER("close_everything");

   KillEventUtility();

   EXIT("close_everything");
}

/***********************************************************************
 * get_joystick_state : returns the current state of the joystick bits.*
 ***********************************************************************/
int32
get_joystick_state( void )
{
   ControlPadEventData cpaddata;

   ENTER("get_joystick_state");

   cpaddata.cped_ButtonBits=0;

   GetControlPad(1,0,&cpaddata);

   EXIT("get_joystick_state");

   return ( cpaddata.cped_ButtonBits );
}

/***********************************************************************
 * getout : exits the program                                          *
 ***********************************************************************/
void
getout(rvalue)

int32 rvalue;
{
   ENTER("getout");

   FadeToBlack( &Screencontext,FADE_FRAMECOUNT );
   close_everything();
   printf("exiting : %ld\n",rvalue);

   exit((int)rvalue);
}

/**************************************************************************
 * init_cels : initializes any cels needed                                *
 **************************************************************************/
bool
init_cels(void)
{
   bool rvalue;

   rvalue = TRUE;
	
   ENTER("init_cels");

   Ccbs[0]=LoadCel(Celfile1,MEMTYPE_CEL);

   if(!(Ccbs[0]))
   {
      printf("unable to load cel file %s\n",Celfile1);
      getout(GENERIC_ERROR);
   }

   Ccbs[1]=LoadCel(Celfile2,MEMTYPE_CEL);

   if(!(Ccbs[1]))
   {
      printf("unable to load cel file %s\n",Celfile2);
      getout(GENERIC_ERROR);
   }

   EXIT("init_cels");

   return(rvalue);
}

IOInfo ioInfo;

/**************************************************************************
 * init_graphics : uses the Lib3DO stuff                              *
 **************************************************************************/
bool
init_graphics(void)
{
   int i;
   int32 width,height;

   ENTER("init_graphics");

   if(!OpenGraphics(&Screencontext,2))return(FALSE);

   for(i=0;i<2;i++)
   {
	  BitmapItems[i] = Screencontext.sc_BitmapItems[i];
      Bitmaps[i] = Screencontext.sc_Bitmaps[i];

        DisableHAVG( BitmapItems[i] );
        DisableVAVG( BitmapItems[i] );

        SetCEControl(BitmapItems[i], 0xffffffff, ASCALL);

        //EnableHAVG( BitmapItems[i] );
        //EnableVAVG( BitmapItems[i] );
   }

   width = Bitmaps[0]->bm_Width;
   height = Bitmaps[0]->bm_Height;


	gVRAMIOReq = CreateVRAMIOReq();		/* Obtain an IOReq for all SPORT operations */

memset(&ioInfo,0,sizeof(ioInfo));
ioInfo.ioi_Command = FLASHWRITE_CMD;
ioInfo.ioi_CmdOptions = 0xffffffff;
ioInfo.ioi_Offset = 0x000f000f;
ioInfo.ioi_Recv.iob_Buffer = Bitmaps[0]->bm_Buffer;
ioInfo.ioi_Recv.iob_Len = width*height*2;

    vsyncItem = GetVBLIOReq();

   EXIT("init_graphics");

   return(TRUE);
}

int whereInit = 0;

void blink()
{
    int i;
    int c = rand();
    for (i=0; i<8; i++)
        Bitmaps[0]->bm_Buffer[i + (whereInit & 3) * 4 * SCREEN_WIDTH + whereInit * 8] = c;
    DisplayScreen( Screencontext.sc_Screens[0], 0 );
    whereInit++;
}

/**************************************************************************
 * init_stuff : initializes everything                                    *
 **************************************************************************/
void
init_stuff(void)
{
    ENTER("init_stuff");

    init_system();

    init_graphics();

    memset(Bitmaps[0]->bm_Buffer, 0, 320*240*2);

    blink();
    
    init_cels(); blink();

    Init3DLib(MONO_VIEW,0); blink();
    initMathUtil(); blink();

//   BackPic=(ubyte *)LoadImage(Backdrop_file,NULL,NULL,&Screencontext);
    init_worlds(); blink();

    initFonts(); blink();
    initTimer(); blink();

	initEngine(); blink();
	initTextures(); blink();

    plasmaInit(); blink();
    blobsInit(); blink();
    rotblobsInit(); blink();
    starsInit(); blink();
    sphereInit(); blink();
    fireInit(); blink();
    bumpInit(); blink();
    polarInit(); blink();
    cloudsInit(); blink();
    testInit(); blink();
    linetestsInit(); blink();
    dotsInit(); blink();
	
	sceneTestInit(); blink();
    sceneGridInit(); blink();

    EXIT("init_stuff");
}

/**************************************************************************
 * init_system : initializes main system level stuff, libraries, etc.     *
 **************************************************************************/
void
init_system(void)
{
   ENTER("init_system");

   if(OpenMathFolio()<0)
   {
      printf("\n\nunable to open math folio!\n\n");
      getout(GENERIC_ERROR);
   }

   if(OpenGraphicsFolio()<0)
   {
      printf("unable to open graphics folio!\n");
      getout(GENERIC_ERROR);
   }


   if(InitEventUtility(1,0,LC_Observer)<0)
   {
      printf("unable to open the event broker\n");
      getout(GENERIC_ERROR);
   }
   
   if(OpenAudioFolio()<0)
   {
      printf("unable to open Audiofolio!\n");
      getout(GENERIC_ERROR);
   }    
   
//   ChangeInitialDirectory(NULL, NULL, TRUE);

    //sportItem = OpenNamedDevice("sport", 0);

   EXIT("init_system");
}

void SetQuadFromPosAndSize(Point *aQuad, int32 xPos, int32 yPos, int32 width, int32 height)
{
	aQuad[kQuadBL].pt_X = aQuad[kQuadTL].pt_X = xPos;
	aQuad[kQuadTR].pt_Y = aQuad[kQuadTL].pt_Y = yPos;
	aQuad[kQuadBR].pt_X = aQuad[kQuadTR].pt_X = xPos + width;
	aQuad[kQuadBL].pt_Y = aQuad[kQuadBR].pt_Y = yPos + height;
}

void drawAvailMem(MemInfo *mi, int yp)
{
    drawNumber(0, yp, mi->minfo_SysFree);
    drawNumber(0, 8 + yp, mi->minfo_SysLargest);
    drawNumber(0, 16 + yp, mi->minfo_TaskFree);
    drawNumber(0, 24 + yp, mi->minfo_TaskLargest);
}


int part = PART_PLASMA;

bool LeftPadPressed = false;
bool RightPadPressed = false;
bool LeftPadPressedOnce = false;
bool RightPadPressedOnce = false;

bool PressedA = false;
bool PressedAonce = false;

/***********************************************************************
 * main_loop :                                                         *
 ***********************************************************************/
int32
main_loop(void)
{
    int gridWidth = 4;
    int gridHeight = 4;
    int px, py, zx, zy;

   int32 rvalue;
   int32 screen_select;        /* determines which screen to write to */
   uint32 counter;

    int x;
    int y;

    CCB *proceduralEffectCel;

    //ubyte *ptr = &BackPic[0];

    //ubyte *ptr2 = &Screencontext.sc_Bitmaps[screen_select]->bm_Buffer[0];	
    //ubyte *ptr2 = &BackPic[0];	

    lineList *lnLst0;

    rvalue = screen_select = counter = 0;

   ENTER("main_loop");

    //ReadAFile("3d_data/sw32.cel", Ccbs[0]->ccb_SourcePtr, 2136);

    //InitFont();

    //musix();

    AvailMem(mi1, MEMTYPE_ANY);
    for(;;)
    {
        /*** if the user has pressed the START button, exit ***/
        if(process_joystick()==START_BUTTON)goto DONE;

// =================== 2d Stuff

    if (RightPadPressedOnce) part = (part + 1) % PARTS_NUM;
    if (LeftPadPressedOnce) part--;
    if (part < 0) part = PARTS_NUM - 1;
    if (PressedAonce) vsync = !vsync;

        switch(part)
        {
            case PART_FIRE:
                sceneGridRun(BitmapItems[screen_select], buttUp, buttDown, buttB, buttC, counter, false);
                lnLst0 = (lineList*)getLinesList();
                fireRun(BitmapItems[screen_select], lnLst0, counter);
            break;

            case PART_PLASMA:
                plasmaRun(BitmapItems[screen_select], counter);
            break;

            case PART_SPHERE:
                sphereRun(BitmapItems[screen_select], counter);
            break;

            case PART_BLOBS:
                blobsRun(BitmapItems[screen_select], counter);
            break;

            case PART_ROTOBLOBS:
                rotblobsRun(BitmapItems[screen_select], counter);
            break;

            case PART_STARS:
                starsRun(BitmapItems[screen_select], counter);
            break;

            case PART_3D:
                render_world(BitmapItems[ screen_select ],0,Camera_id,World_id);
            break;

            case PART_POLAR:
                polarRun(BitmapItems[screen_select], counter);
            break;

            case PART_BUMP:
                bumpRun(BitmapItems[screen_select], counter, true);
            break;

            case PART_CLOUDS:
                cloudsRun(BitmapItems[screen_select], buttB, buttC, counter);
            break;

            case PART_TEST:
                testRun(BitmapItems[screen_select], buttB, buttC, counter);
            break;

            case PART_LINETESTS:
                linetestsRun(BitmapItems[screen_select], buttB, Bitmaps[screen_select]->bm_Buffer, counter);
            break;

            case PART_DOTS:
                dotsRun(Bitmaps[screen_select]->bm_Buffer, counter);
            break;
			
			case PART_SCENE_TEST:
                proceduralEffectCel = bumpRun(BitmapItems[screen_select], counter, false);
				sceneTestRun(BitmapItems[screen_select], proceduralEffectCel, counter);
			break;

			case PART_SCENE_GRID:
				sceneGridRun(BitmapItems[screen_select], buttUp, buttDown, buttB, buttC, counter, true);
			break;
        }

        showFPS(); // 24, 8

        // 64,80
        //drawAvailMem(mi0, 8);
        //drawAvailMem(mi1, 48);

        renderTextSpace(BitmapItems[screen_select]);

        //DrawFont(BitmapItems[screen_select]);

        rvalue = DisplayScreen( Screencontext.sc_Screens[screen_select], 0 );
        if (vsync) WaitVBL(vsyncItem, 1);

        if(rvalue<0)
        {
            printf("DisplayScreen failed!\n");
            goto DONE;
        }

        /*** alternates buffers ***/

        screen_select = (screen_select + 1) & 1;

        ioInfo.ioi_Recv.iob_Buffer = Bitmaps[screen_select]->bm_Buffer;
        DoIO(gVRAMIOReq,&ioInfo);

        counter++;
        nframe++;
   }

   EXIT("main_loop");

DONE:
   return(rvalue);
}


/**************************************************************************
 * process_joystick :                                                     *
 **************************************************************************/
int32
process_joystick(void)
{
   int32 rvalue;
   int32 joybits;
   static int32 delta_y=0, delta_x=0, delta_camera_angle=0;
   static int32 pixc=SOLID_CEL;

   rvalue = 0;
	
   ENTER("process_joystick");

   joybits=get_joystick_state();

   if (joybits&ControlStart)
   {
      rvalue=START_BUTTON;
      goto DONE;
   }

  /*** toggle between worlds ***/
    if(joybits&ControlB)
        buttB = 1;
    if(joybits&ControlC)
        buttC = 1;

   if(joybits&ControlA)
   {
        PressedAonce = !PressedA;
        PressedA = true;
   }
    else PressedA = false;

    if (joybits & ControlRightShift)
    {
        RightPadPressedOnce = !RightPadPressed;
        RightPadPressed = true;
    }
    else RightPadPressedOnce = RightPadPressed = false;

    if (joybits & ControlLeftShift)
    {
        LeftPadPressedOnce = !LeftPadPressed;
        LeftPadPressed = true;
    }
    else LeftPadPressedOnce = LeftPadPressed = false;

   if(joybits&ControlB)
   {
        pixc=TRANSLUCENT_CEL;
      //delta_camera_angle=(1<<15);
   }

  /*** toggle the animation on and off ***/

   if(joybits&ControlC)
   {
      Rotate_stuff=0;
   }

  /*** start motion on the first click, stop on the second ***/

   if(joybits&ControlDown)
   {
        buttDown = 1;
       delta_y=-(1<<12);
   }

   if(joybits&ControlUp)
   {
        buttUp = 1;
       delta_y=(1<<12); 
   }

   if(joybits&ControlLeft)
   {
       delta_x=-(1<<12); 
   }

   if(joybits&ControlRight)
   {
       delta_x=(1<<12); 
   }

   if(!joybits)
   {
      buttB = 0;
      buttC = 0;
      buttUp = 0;
      buttDown = 0;
      pixc=SOLID_CEL;
      delta_x=0;
      delta_y=0;
      Rotate_stuff=1;
      delta_camera_angle=0;
   }

   Ccbs[0]->ccb_PIXC=pixc;

   View_X+=delta_x;
   View_Y+=delta_y;
   Camera_angle+=delta_camera_angle;

   EXIT("process_joystick");

DONE:
   return(rvalue);
}


/**************************************************************************
 * main :                                                                 *
 **************************************************************************/
int
main()
{
    int32 rvalue = 0;
    mi0 = malloc(sizeof(MemInfo));
    mi1 = malloc(sizeof(MemInfo));

    AvailMem(mi0, MEMTYPE_ANY);
    init_stuff();

    main_loop();

    getout(rvalue);
}


