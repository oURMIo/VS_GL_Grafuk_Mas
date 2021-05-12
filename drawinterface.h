/* Элементы интерфейса */

#ifndef DRAWINTERFACE_H
#define DRAWINTERFACE_H

/**** TYPES ***********************************************************************************************/
typedef struct
{
    float x1, y1, x2, y2;
}  float4;

typedef struct
{
    float left, right, top, bottom;
} CRect;


/**** DEFINES **************************************************************************************************/
/** decide if make defines or constants!!!!!!!!!!!!!!!!!   ??????????????????????????????????????????????  */

#define COLOR_group_DARK {0.6, 0.6, 0.6}
#define COLOR_group_LIGHT {0.9, 0.9, 0.9}


/** SLIDER ****/
const float COLOR_SLIDER_DARK[3] = COLOR_group_DARK;
const float COLOR_SLIDER_LIGHT[3] = COLOR_group_LIGHT;

// Slider coordinates
const float sld_group_top = 30.0;
const float sld_group_dy = 40.0;
const float sld_group_h = 20.0;
const float SLIDER_1_TOP = sld_group_top - 0 * sld_group_dy;
const float SLIDER_2_TOP = sld_group_top - 1 * sld_group_dy;
const float SLIDER_3_TOP = sld_group_top - 2 * sld_group_dy;
const float SLIDER_4_TOP = sld_group_top - 3 * sld_group_dy;
const float SLIDER_5_TOP = sld_group_top - 4 * sld_group_dy;
// элементы находятся в области меню
const float4 SLIDER_1_RECT_GL = {SCREEN_HALF_X_GL + 10.0, SLIDER_1_TOP, SCREEN_HALF_X_GL + SCREEN_MX_GL - 10.0, SLIDER_1_TOP + sld_group_h};
const float4 SLIDER_2_RECT_GL = {SCREEN_HALF_X_GL + 10.0, SLIDER_2_TOP, SCREEN_HALF_X_GL + SCREEN_MX_GL - 10.0, SLIDER_2_TOP + sld_group_h};
const float4 SLIDER_3_RECT_GL = {SCREEN_HALF_X_GL + 10.0, SLIDER_3_TOP, SCREEN_HALF_X_GL + SCREEN_MX_GL - 10.0, SLIDER_3_TOP + sld_group_h};
const float4 SLIDER_4_RECT_GL = {SCREEN_HALF_X_GL + 10.0, SLIDER_4_TOP, SCREEN_HALF_X_GL + SCREEN_MX_GL - 10.0, SLIDER_4_TOP + sld_group_h};
const float4 SLIDER_5_RECT_GL = {SCREEN_HALF_X_GL + 10.0, SLIDER_5_TOP, SCREEN_HALF_X_GL + SCREEN_MX_GL - 10.0, SLIDER_5_TOP + sld_group_h};


/** EDITBOX ****/
const float edb_group_top = 130.0;
const float edb_group_dy = 30.0;
const float edb_group_h = 20;
const float EDITBOX_1_TOP = edb_group_top;
const float4 EDITBOX_1_RECT = {SCREEN_HALF_X_GL + 10.0, EDITBOX_1_TOP, SCREEN_HALF_X_GL + SCREEN_MX_GL - 10.0, EDITBOX_1_TOP + edb_group_h};


/** RADIOBUTTON ****/
const float COLOR_RADIO_OUTER[3] = COLOR_group_DARK;
const float COLOR_RADIO_INNER[3] = COLOR_group_LIGHT;
const float COLOR_RADIO_SELECTED[3] = COLOR_group_DARK;

#define RADIO_R_OUTER   6.0
#define RADIO_R_INNER   4.0
#define RADIO_R_SELECTED 2.0


const float radiogroup1_top = 180;
const float radiogroup2_top = -160;
const float radiogroup_dy  = 20;
const float radiogroup1_n  = 4;
const float radiogroup_xc = SCREEN_HALF_X_GL + 10.0 + RADIO_R_OUTER;
const float RADIOGROUP_1_COORD3[3] = {radiogroup1_top, radiogroup_dy, radiogroup_xc};
const float RADIOGROUP_2_COORD3[3] = {radiogroup2_top, radiogroup_dy, radiogroup_xc};





/**** classes **************************************************************************************************/

class CBaseVisualElement
{
public:
    bool isClickInsideRect(float4 coord, int x, int y, int varScreenKX, int varScreenKY);
};

/*-------------------------------------------------------------------------------------------------------------*/
class CSlider : public CBaseVisualElement
{
public:
    CRect rectPX;
    //-------------------------------------------------------------------------------------------------
    CSlider(float4 coordGL, float newMarkRatio, int varScreenW); // constructor setting dimensions
    void DrawHorizontal();
    void MarkMoveHorizontal(int clickPx, float varScreenW);
    float markRatio; // it's its value // mark ratio 0...1    (left-------mark-------------right )
    //-------------------------------------------------------------------------------------------------
    void ImplementNVert(float x, float varScreenW, CGraph graph1);  // vertexes quantity
    void ImplementNEdges(float x, float varScreenW, CGraph graph1); // edges quantity
private:
    CRect rectGL; // overall dimensions
    float mCoordGL;   //  for horizontal or vertical - I need a direction flag
};

/*-------------------------------------------------------------------------------------------------------------*/
class CEditBox : public CBaseVisualElement
{
public:
    CEditBox(float4 coordGL);
    //CString value;

private:
    CRect rectGL; // overall dimensions
};

typedef char tstr6[7];
/*-------------------------------------------------------------------------------------------------------------*/
class CRadioButtonGroup : public CBaseVisualElement
{
public:

	CRadioButtonGroup(const float *param, int n_elem_p, int n_selected_p, tstr6 *str6_in);
    void DrawGroup();
    int setSelectedElemIfClicked(int x, int y, int varScreenW, int varScreenH);

private:
    //CRect rectGL;
    float top, dy, xc;
    int n_elem;
    int n_selected;
    struct {
        //const float outer = RADIO_R_OUTER;
		//const float inner = RADIO_R_INNER;
		//const float selected = RADIO_R_SELECTED;
		float outer;
		float inner;
		float selected;
    } r;
	tstr6 str6[4];
    void DrawButton(int num, bool isSelected);
};


/**** PROTOTYPES ***********************************************************************************************/
void DrawTextMenu();


#endif // DRAWINTERFACE_H


