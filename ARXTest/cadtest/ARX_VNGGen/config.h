#pragma once

/*
 * �ڵ�֮�����С���,
 * ��dot��Ĭ��Ϊ0.25Ӣ��
 * ������Ĭ��Ϊ0.5Ӣ��->12.7����
 */
#define DEF_NODE_SEP 12.7

/*
 * ��ֱ���֮�����С���
 * ��dot��Ĭ��Ϊ0.5Ӣ��
 * �ڱ�����Ĭ��Ϊ1Ӣ��->25.4����
 */
#define DEF_RANK_SEP 25.4

/* Ĭ��ͼ����/�߱�ֵ */
#define DEF_GRAPH_RATIO 3

/* Ĭ��ͼ�����800���� */
#define DEF_GRAPH_WIDTH 600

/* Ĭ��ͼ���߶�600���� */
#define DEF_GRAPH_HEIGHT 800

/*
 * �Ƿ�ʹ��Ĭ�ϵĿ������
 * ��������ratio�Լ����
 */
#define DEF_GRAPH_USE_DEF_WH 1

/* 
 * �ڵ�Ŀ�� 
 * ��dot��Ĭ��Ϊ0.75Ӣ��
 */
#define DEF_NODE_WIDTH 19.05

/*
 * �ڵ�ĸ߶�
 * ��dot��Ĭ��Ϊ0.5Ӣ��
 */
#define DEF_NODE_HEIGHT 12.7

/*
 * dot������߶�Ĭ��Ϊ14pt
 * �����mm==>14/72*25.4 = 4.94
 */
#define DEF_NODE_TEXT_HEIGHT 4.94


/*
 * ��dot��, ��ͷ���Ĭ��Ϊ1Ӣ��
 * �ڱ�������Ĭ��Ϊ4����
 */
#define DEF_ARROW_WIDTH 4

/* 
 * ��ͷĬ�ϳ���
 * �ڱ�������Ĭ��Ϊ9����
 */
#define DEF_ARROW_LENGTH 9

/*
 * dot������߶�Ĭ��Ϊ14pt
 * �����mm==>14/72*25.4 = 4.94
 */
#define DEF_EDGE_TEXT_HEIGHT 4.94

/* Ĭ�ϲ���ʾ��֧��� */
#define DEF_NEED_EDGE 0

/* ��cad�ʵ��д洢ͨ������ͼ���Ʋ�����Ҳ��dot��Ҫ�Ĳ��� */

#define VNG_PARAM_DICT _T("ͨ������ͼ���Ʋ����ʵ�")

#define NODE_SEP_KEY _T("ˮƽ��С���")
#define RANK_SEP_KEY _T("��ֱ��С���")

#define GRAPH_RATIO_KEY _T("ͼ�ο�߱�ֵ")
#define GRAPH_WIDTH_KEY _T("ͼ�ο��")
#define GRAPH_HEIGHT_KEY _T("ͼ�θ߶�")
#define GRAPH_USE_DEF_WH_KEY _T("�Ƿ�ʹ��Ĭ�ϵĿ������")

#define NODE_WIDTH_KEY _T("�ڵ���")
#define NODE_HEIGHT_KEY _T("�ڵ�߶�")
#define NODE_TEXT_HEIGHT_KEY _T("�ڵ��ע����߶�")

#define ARROW_WIDTH_KEY _T("��ͷ���")
#define ARROW_LENGTH_KEY _T("��ͷ����")
#define EDGE_TEXT_HEIGHT_KEY _T("��֧��ע����߶�")

#define NEED_EDGE_KEY _T("��ʾ��֧���")