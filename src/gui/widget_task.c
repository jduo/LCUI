﻿/* ***************************************************************************
 * widget_task.c -- LCUI widget task module.
 * 
 * Copyright (C) 2014 by Liu Chao <lc-soft@live.cn>
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ***************************************************************************/
 
/* ****************************************************************************
 * widget_task.c -- LCUI部件任务处理模块
 *
 * 版权所有 (C) 2014 归属于 刘超 <lc-soft@live.cn>
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ***************************************************************************/

#define __IN_WIDGET_TASK_SOURCE_FILE__

#include <LCUI_Build.h>
#include <LCUI/LCUI.h>
#include <LCUI/misc/rbtree.h>
#include <LCUI/widget_build.h>

typedef struct TaskRecord {
	LCUI_BOOL	is_valid;	/**< 是否有任务 */
	LCUI_WidgetTask	data;		/**< 任务数据 */
} TaskRecord;

struct LCUI_WidgetTaskBoxRec_ {
	int		i;				/**< 当前使用的记录号 */
	LCUI_BOOL	for_self;			/**< 标志，指示当前部件是否有待处理的任务 */
	LCUI_BOOL	for_children;			/**< 标志，指示是否有待处理的子级部件 */
	TaskRecord	buffer[2][WTT_TOTAL_NUM];	/**< 两个记录缓存 */
};

/** 处理位置移动 */
static void HandleMove( LCUI_Widget w, LCUI_WidgetTask *t )
{
	LCUI_Rect rect;
	Widget_GetRect( w, &rect );
	Widget_InvalidateArea( w->parent, &rect );
	rect.x = t->move.x;
	rect.y = t->move.y;
	Widget_InvalidateArea( w->parent, &rect );
}

/** 处理尺寸调整 */
static void HandleResize( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理可见性 */
static void HandleVisibility( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理透明度 */
static void HandleOpacity( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理阴影（标记阴影区域为脏矩形，但不包括主体区域） */
static void HandleShadow( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理主体刷新（标记主体区域为脏矩形，但不包括阴影区域） */
static void HandleBody( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理刷新（标记整个部件区域为脏矩形） */
static void HandleRefresh( LCUI_Widget w, LCUI_WidgetTask *t )
{
	
}

/** 处理销毁任务 */
static void HandleDestroy( LCUI_Widget w, LCUI_WidgetTask *t )
{
	/* 销毁先子部件，最后销毁自己 */
	// code ...
	/* 向父级部件添加相关任务 */
	// code ...
}

/** 添加任务 */
void Widget_AddTask( LCUI_Widget widget, LCUI_WidgetTask *data )
{
	TaskRecord *buffer;
	buffer = widget->task->buffer[widget->task->i == 1 ? 0:1];
	buffer[data->type].is_valid = TRUE;
	buffer[data->type].data = *data;
	widget->task->for_self = TRUE;
	widget = widget->parent;
	/* 向没有标记的父级部件添加标记 */
	while( widget && !widget->task->for_children ) {
		widget->task->for_children = TRUE;
		widget = widget->parent;
	}
}

typedef void (*callback)(LCUI_Widget, LCUI_WidgetTask*);

static callback task_handlers[WTT_TOTAL_NUM];

/** 映射任务处理器 */
static void MapTaskHandler(void)
{
	/** 不能用C99标准中的初始化方式真蛋疼... */
	task_handlers[WTT_SHOW] = HandleVisibility;
	task_handlers[WTT_MOVE] = HandleMove;
	task_handlers[WTT_RESIZE] = HandleResize;
	task_handlers[WTT_SHADOW] = HandleShadow;
	task_handlers[WTT_OPACITY] = HandleOpacity;
	task_handlers[WTT_BODY] = HandleBody;
	task_handlers[WTT_REFRESH] = HandleRefresh;
}

/** 初始化 LCUI 部件任务处理功能 */
void LCUIWidget_Task_Init(void)
{
	MapTaskHandler();
}

/** 销毁（释放） LCUI 部件任务处理功能的相关资源 */
void LCUIWidget_Task_Destroy(void)
{

}

/** 初始化部件的任务处理 */
void Widget_InitTaskBox( LCUI_Widget widget )
{
	int i;
	widget->task = (LCUI_WidgetTaskBox)
	malloc(sizeof(struct LCUI_WidgetTaskBoxRec_));
	widget->task->for_children = FALSE;
	widget->task->for_self = FALSE;
	for( i=0; i<WTT_TOTAL_NUM; ++i ) {
		widget->task->buffer[0][i].is_valid = FALSE;
		widget->task->buffer[1][i].is_valid = FALSE;
	}
}

/** 销毁（释放）部件的任务处理功能的相关资源 */
void Widget_DestroyTaskBox( LCUI_Widget widget )
{
	free( widget->task );
	widget->task = NULL;
}

/** 处理部件的各种任务 */
static void Widget_ProcTask( LCUI_Widget w )
{
	int i, n;
	TaskRecord *buffer;
	/* 如果该部件有任务需要处理 */
	if( w->task->for_self ) {
		w->task->for_self = FALSE;
		/* 切换前后台记录 */
		w->task->i = w->task->i == 1 ? 0:1;
		buffer = w->task->buffer[w->task->i];
		/* 如果该部件需要销毁，其它任务就不用再处理了 */
		if( buffer[WTT_DESTROY].is_valid ) {
			task_handlers[WTT_DESTROY]( w, NULL );
			return;
		}
		for( i=0; i<WTT_USER; ++i ) {
			if( buffer[i].is_valid ) {
				task_handlers[i](w, &buffer[i].data);
			}
		}
		/* 如果有用户自定义任务 */
		if( buffer[WTT_USER].is_valid ) {	
			LCUI_WidgetClass *wc;
			wc = LCUIWidget_GetClass( w->type_name );
			wc->task_handler(w, &buffer[i].data);
		}
		/* 只留了一个位置用于存放用户自定义任务，以后真有需求再改吧 */
	}
	/* 如果子级部件中有待处理的部件，则递归进去 */
	if( w->task->for_children ) {
		LCUI_Widget child;
		w->task->for_children = FALSE;
		n = LinkedList_GetTotal( &w->children );
		LinkedList_Goto( &w->children, 0 );
		for( i=0; i<n; ++i ) {
			child = (LCUI_Widget)LinkedList_Get( &w->children );
			LinkedList_ToNext( &w->children );
			Widget_ProcTask( child );
		}
	}
}

/** 处理一次当前积累的部件任务 */
void LCUIWidget_Task_Step(void)
{
	Widget_ProcTask( LCUIRootWidget );
}