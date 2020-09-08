/* chart.c -- Widget for drawing animated charts
 *
 * Copyright (c) 2018-2020, James Duong <duong.james@gmail.com> All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of LCUI nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <LCUI_Build.h>
#include <LCUI.h>
#include <LCUI/timer.h>
#include <LCUI/gui/metrics.h>
#include <LCUI/gui/widget.h>
#include <LCUI/gui/widget/canvas.h>
#include <LCUI/gui/widget/chart.h>
#include <LCUI/gui/css_parser.h>
#include <LCUI/gui/css_fontstyle.h>

typedef struct ChartRec_ {
	int size;
	int custom_size;
	int timer;
	int angle;
	int duration;
	int start;
	int line_width;
	LCUI_Color color;
	int64_t start_time;
} ChartRec, *Chart;

static struct {
	LCUI_WidgetPrototype proto;
} self;

static void Chart_OnInit(LCUI_Widget w)
{
	Chart chart =
	    Widget_AddData(w, self.proto, sizeof(ChartRec));

	self.proto->proto->init(w);
/*	spinner->line_width = 2;
	spinner->custom_size = 32;
	spinner->size = spinner->custom_size;
	spinner->angle = 90;
	spinner->start = 360;
	spinner->duration = 750;
	spinner->start_time = LCUI_GetTime();
	spinner->timer =
	    LCUI_SetInterval(LCUI_MAX_FRAME_MSEC, (TimerCallback)Chart_OnFrame, w);*/
}

static void Chart_OnDestroy(LCUI_Widget w)
{
	Chart chart = Widget_GetData(w, self.proto);

	LCUITimer_Free(chart->timer);
}

static void Chart_AutoSize(LCUI_Widget w, float *width, float *height,
			     LCUI_LayoutRule rule)
{
	float scale = LCUIMetrics_GetScale();
	Chart chart = Widget_GetData(w, self.proto);

	*width = chart->size / scale;
	*height = chart->size / scale;
}


static void Chart_OnFrame(LCUI_Widget w)
{
	Chart spinner = Widget_GetData(w, self.proto);
	int duration = (int)LCUI_GetTimeDelta(spinner->start_time);

	spinner->start = 360 - (int)(duration * 360.0 / spinner->duration) % 360;
	if (duration >= spinner->duration) {
		spinner->start_time += duration;
	}
	Chart_Render(w);
}

static void Spinner_Render(LCUI_Widget w)
{
	LCUI_Color c;
	LCUI_Color slider_color, border_color;
	LCUI_CanvasContext ctx = Canvas_GetContext(w);
	Chart chart = Widget_GetData(w, self.proto);

	int xi, yi;
	int start = chart->start;
	int end = start - chart->angle;

	double d;
	double r = chart->size / 2.0 - 0.5;
	double inner_r = r - chart->line_width * ctx->scale;
	double outer_smooth_r2 = (r + 1.0) * (r + 1.0);

	border_color = chart->color;
	slider_color = ARGB(0, 0, 0, 0);
	ctx->clearRect(ctx, 0, 0, ctx->width, ctx->height);
	if (ctx->width < chart->size || ctx->height < chart->size) {
		ctx->release(ctx);
		return;
	}
	for (yi = 0; yi < chart->size; ++yi) {
		double y = r - yi;
		double y2 = y * y;

		for (xi = 0; xi < chart->size; ++xi) {
			double x = xi - r;
			double x2 = x * x;

			d = x2 + y2;
			if (d >= outer_smooth_r2) {
				continue;
			}
			if (CheckInSlider(x, y, start, end)) {
				c = slider_color;
			} else {
				c = border_color;
			}
			d = sqrt(d) - r;
			if (d >= 0) {
				c.a = SmoothLeftPixel(&c, d);
				FillPixel(&ctx->buffer, xi, yi, &c);
				continue;
			}
			d = d + r - inner_r;
			if (d < 0) {
				continue;
			}
			if (d < 1.0) {
				c.a = SmoothRightPixel(&c, d);
			}
			FillPixel(&ctx->buffer, xi, yi, &c);
		}
	}
	Widget_InvalidateArea(w, NULL, SV_CONTENT_BOX);
	ctx->release(ctx);
}

void LCUIWidget_AddChart(void)
{
	self.proto = LCUIWidget_NewPrototype("chart", "canvas");
	self.proto->init = Chart_OnInit;
	self.proto->destroy = Chart_OnDestroy;
	self.proto->autosize = Chart_AutoSize;
	LCUI_LoadCSSString("spinner { display: inline-block; }", __FILE__);
}
