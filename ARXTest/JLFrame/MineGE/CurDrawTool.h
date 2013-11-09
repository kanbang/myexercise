#pragma once

class MineGEDraw;

/*
 * 由于每个图元可能包含多个可视化效果
 * 例如：
 *		巷道可能包含可视化效果：双线巷道、单线巷道、宽多段线巷道等等
 *
 * 系统使用"词典"记录每个图元的当前可视化效果
 * 例如：
 *		Tunnel     --> DoubleTunnelDraw        (巷道      -->  双线巷道可视化效果)
 *		Direction  --> PolylineDirectionDraw   (风流方向  -->  多段线风流方向可视化效果)
 *		Gate       --> SimpleGateDraw          (风门      -->  简单的风门可视化效果)
 *		...        --> ...
 */

/*
 * 初始化所有图元的当前可视化效果
 * 注：如果图元的可视化效果已注册(即在词典中该图元类型的key存在)，则保持不变
 */
extern void InitAllCurDraws();

/* 获取图元的当前可视化效果 */
extern bool GetCurDraw(const CString& type, CString& draw);

/* 设置图元的当前可视化效果 */
extern bool SetCurDraw(const CString& type, const CString& draw);
