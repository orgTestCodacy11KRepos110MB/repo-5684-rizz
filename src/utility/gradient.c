#include "rizz/imgui.h"
#include "rizz/utility.h"

static inline void sortkeys(rizz_gradient* gradient)
{
    uint32_t n = gradient->num_keys;
    while (n) {
        uint32_t i, newn = 0;
        for (i = 1; i < n; ++i) {
            if ((gradient->keys[i - 1].t - gradient->keys[i].t) > 0) {
                sx_color tmp_color = gradient->keys[i - 1].color;
                float tmp_t = gradient->keys[i - 1].t;
                gradient->keys[i - 1] = gradient->keys[i];
                gradient->keys[i].color = tmp_color;
                gradient->keys[i].t = tmp_t;
                newn = i;
            }
        }
        n = newn;
    }
}

void gradient__init(rizz_gradient* gradient, sx_color start, sx_color end)
{
    *gradient = (rizz_gradient){
        .keys[0] = { .t = 0.0f, .color = start },
        .keys[1] = { .t = 1.0f, .color = end },
        .num_keys = 2,
    };
}

bool gradient__add_key(rizz_gradient* gradient, sx_color color, float t)
{
    if (gradient->num_keys == RIZZ_GRADIENT_MAX_KEYS)
        return false;    // no room for new keys

    t = sx_clamp(t, 0.02f, 0.98f);    // avoid overlap on first or last key
    uint32_t index = gradient->num_keys;
    gradient->keys[index].color = color;
    gradient->keys[index].t = t;
    gradient->num_keys++;
    sortkeys(gradient);
    return true;
}

bool gradient__move_key(rizz_gradient* gradient, int index, float t)
{
    if (index <= 0 || index >= gradient->num_keys - 1)
        return false;    // dont move first or last key

    t = sx_clamp(t, 0.02f, 0.98f);    // avoid overlap on first or last key
    gradient->keys[index].t = t;
    sortkeys(gradient);
    return true;
}

bool gradient__remove_key(rizz_gradient* gradient, int index)
{
    if (index <= 0 || index >= gradient->num_keys - 1)
        return false;    // dont remove first or last key

    gradient->num_keys--;
    const int key_size = sizeof(gradient->keys[0]);
    sx_memmove(&gradient->keys[index], &gradient->keys[index + 1],
               (gradient->num_keys - index) * key_size);

    return true;
}

sx_color gradient__eval(const rizz_gradient* gradient, float t)
{
    sx_assert(gradient->num_keys);

    if (gradient->num_keys == 1)
        return gradient->keys[0].color;

    int a = 0;
    int b = 1;

    for (int i = 0; i < gradient->num_keys; i++) {
        if (gradient->keys[i].t >= t) {
            b = i;
            break;
        } else {
            a = i;
        }
    }

    sx_color ac = gradient->keys[a].color;
    sx_color bc = gradient->keys[b].color;
    float at = gradient->keys[a].t;
    float bt = gradient->keys[b].t;
    t = (t - at) / (bt - at);
    t = sx_clamp(t, 0.0f, 1.0f);
    return (sx_color){
        .r = (uint8_t)sx_lerp((float)ac.r, (float)bc.r, t),
        .g = (uint8_t)sx_lerp((float)ac.g, (float)bc.g, t),
        .b = (uint8_t)sx_lerp((float)ac.b, (float)bc.b, t),
        .a = (uint8_t)sx_lerp((float)ac.a, (float)bc.a, t),
    };
}

void gradient__edit(const rizz_api_imgui* gui, const char* label, rizz_gradient* gradient)
{
    gui->PushIDStr(label);
    sx_vec2 rpos, rsize, mpos;
    gui->GetMousePos(&mpos);
    gui->GetCursorScreenPos(&rpos);
    rsize = sx_vec2f(gui->CalcItemWidth(), 24);
    ImDrawList* dlst = gui->GetWindowDrawList();
    rpos = sx_vec2_addf(rpos, 2.0f);
    rsize = sx_vec2_subf(rsize, 4.0f);

    int count = gradient->num_keys;

    // draws checker
    {
        sx_vec2 p_min = rpos;
        sx_vec2 p_max = sx_vec2_add(rpos, rsize);
        gui->RenderColorRectWithAlphaCheckerboard(dlst, p_min, p_max, 0, rsize.y / 2.0f,
                                                  SX_VEC2_ZERO, 0, 0);
    }

    // draw gradient rects
    for (int i = 0; i < count - 1; i++) {
        sx_color c1 = gradient->keys[i].color;
        sx_color c2 = gradient->keys[i + 1].color;
        float gx = gradient->keys[i].t;
        float gw = gradient->keys[i + 1].t - gx;

        sx_vec2 p_min = sx_vec2f(rpos.x + gx * rsize.x, rpos.y);
        sx_vec2 p_max = sx_vec2f(rpos.x + (gx + gw) * rsize.x, rpos.y + rsize.y);
        gui->ImDrawList_AddRectFilledMultiColor(dlst, p_min, p_max, c1.n, c2.n, c2.n, c1.n);
    }

    // draw mid line
    {
        sx_vec2 p1 = sx_vec2f(rpos.x, rpos.y + rsize.y * 0.5f);
        sx_vec2 p2 = sx_vec2f(rpos.x + rsize.x, rpos.y + rsize.y * 0.5f);
        gui->ImDrawList_AddLine(dlst, p1, p2, SX_COLOR_BLACK.n, 3.0f);
        gui->ImDrawList_AddLine(dlst, p1, p2, SX_COLOR_WHITE.n, 1.0f);
    }

    // draw keys
    int del_i = -1;
    for (int i = 0; i < count; i++) {
        gui->PushIDInt(i);
        sx_color ic = gradient->keys[i].color;
        float it = gradient->keys[i].t;
        sx_vec2 kpos = sx_vec2f(rpos.x + it * rsize.x, rpos.y + rsize.y * 0.5f);


        gui->ImDrawList_AddCircleFilled(dlst, kpos, 4, SX_COLOR_BLACK.n, 16);
        gui->ImDrawList_AddCircleFilled(dlst, kpos, 3, SX_COLOR_WHITE.n, 16);
        gui->ImDrawList_AddCircleFilled(dlst, kpos, 2, ic.n, 16);

        gui->SetCursorScreenPos(sx_vec2_subf(kpos, 5));
        gui->InvisibleButton("grad-inv-btn", sx_vec2f(10, 10));
        if (gui->IsItemActive()) {
            float t = (mpos.x - rpos.x) / rsize.x;
            gradient__move_key(gradient, i, t);

            if (mpos.y > rpos.y + rsize.y)    // warn user this key will be removed
                gui->ImDrawList_AddCircleFilled(dlst, kpos, 8, sx_color4u(255, 0, 0, 220).n, 16);
        }

        if (gui->IsItemDeactivated() && mpos.y > rpos.y + rsize.y) {
            del_i = i;    // mark i for remove later
        }

        if (gui->BeginPopupContextItem("grad-key-popup", 1)) {
            sx_vec4 c = sx_color_vec4(gradient->keys[i].color);
            ImGuiColorEditFlags_ flags =
                ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar |
                ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf;
            gui->ColorPicker4("", c.f, flags, NULL);
            gradient->keys[i].color = (sx_color){
                .r = (uint8_t)(sx_clamp(c.x, 0, 1) * 255),
                .g = (uint8_t)(sx_clamp(c.y, 0, 1) * 255),
                .b = (uint8_t)(sx_clamp(c.z, 0, 1) * 255),
                .a = (uint8_t)(sx_clamp(c.w, 0, 1) * 255),
            };
            gui->PushStyleColorU32(ImGuiCol_Button, SX_COLOR_RED.n);
            gui->PushStyleColorU32(ImGuiCol_ButtonHovered, sx_color4u(255, 80, 80, 255).n);
            gui->PushStyleColorU32(ImGuiCol_ButtonActive, sx_color4u(255, 80, 80, 255).n);
            gui->Dummy(sx_vec2f(0, 10));
            if (gui->Button("Remove", sx_vec2f(-1, 20))) {
                del_i = i;    // mark i for remove later
                gui->CloseCurrentPopup();
            }
            gui->PopStyleColor(3);
            gui->EndPopup();
        }
        gui->PopID();
    }

    if (del_i != -1)
        gradient__remove_key(gradient, del_i);

    // invisible button for add new key
    gui->SetCursorScreenPos(rpos);
    if (gui->InvisibleButton("grad-add-key", rsize)) {
        float t = (mpos.x - rpos.x) / rsize.x;
        sx_color c = gradient__eval(gradient, t);
        gradient__add_key(gradient, c, t);
    }

    // label
    {
        // sx_vec2 cpos;
        // gui->GetCursorScreenPos(&cpos);
        // gui->SetCursorScreenPos(sx_vec2f(rpos.x + rsize.x, rpos.y + 100));
        gui->SameLine(0, 4);
        gui->Text(label);
        // gui->SetCursorScreenPos(cpos);
    }
    gui->PopID();
}