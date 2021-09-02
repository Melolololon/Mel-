#include "RouteSearch.h"
#include"LibMath.h"
#include"ModelData.h"

using namespace MelLib;

#ifdef _DEBUG
std::vector<AStarNode*>AStarNode::pNodes;

MelLib::AStarNode::AStarNode()
{
	pNodes.push_back(this);

	//�p�x�ݒ�߂�ǂ�����{�b�N�X�ɂ��Ă�
	nodeModel.Create(ModelData::Get(ShapeType3D::BOX), nullptr);
	//nodeModel.SetScale(size)
}

MelLib::AStarNode::~AStarNode()
{
	for (int i = 0, size = pNodes.size(); i < size; i++) 
	{
		if (pNodes[i] == this) 
		{
			pNodes.erase(pNodes.begin() + i);
			break;
		}
	}
}

void MelLib::AStarNode::Draw()
{
	nodeModel.Draw();
}

void MelLib::AStarNode::AllDraw()
{
	for(auto& p : pNodes)
	{
		p->Draw();
	}
}

#endif // _DEBUG


#pragma region �ŒZ�o�H

void RouteSearch::SetAStarNodePosition
(
	const Vector2& leftUpPos,
	const Vector2& rightDownPos,
	const UINT& nodeNumX,
	const UINT& nodeNumY,
	std::vector< std::vector<AStarNode>>& nodes,
	const bool upPlus
)
{
	nodes.clear();

	//�}�X�̃T�C�Y�����߂�
	Vector2 size = rightDownPos - leftUpPos;
	size.x = abs(size.x);
	size.y = abs(size.y);
	if (nodeNumX >= 1) size.x /= nodeNumX - 1;
	if (nodeNumY >= 1) size.y /= nodeNumY - 1;

	//�m�[�h���T�C�Y
	nodes.resize(nodeNumY, std::vector<AStarNode>(nodeNumX));

	//�m�[�h�̍��W���Z�b�g���Ă���
	Vector2 addPos = 0;
	for (int y = 0; y < nodeNumY; y++)
	{
		for (int x = 0; x < nodeNumX; x++)
		{
			if (upPlus)addPos = Vector2(size.x * x, size.y * -y);
			else addPos = Vector2(size.x * x, size.y * y);
			nodes[y][x].position = leftUpPos + addPos;
		}
	}

}

void RouteSearch::SetAStarNodeHitObjectNodeFlag
(
	const std::vector<Vector2>& hitObjectsPos,
	const std::vector<Vector2>& hitObjectsSize,
	std::vector< std::vector<AStarNode>>& nodes
)
{
	//�T�C�Y�����߂�
	Vector2 nodeSize = 0;
	nodeSize.x = abs(nodes[0][0].position.x - nodes[0][1].position.x);

	float smallPos = 0.0f;
	float bigPos = 0.0f;
	if (nodes[0][0].position.y >= nodes[1][0].position.y)
	{
		bigPos = nodes[0][0].position.y;
		smallPos = nodes[1][0].position.y;
	}
	else
	{
		bigPos = nodes[1][0].position.y;
		smallPos = nodes[0][0].position.y;
	}
	nodeSize.y = abs(smallPos - bigPos);

	//����
	auto nodesXNum = nodes[0].size();
	auto nodesYNum = nodes.size();
	auto hitObjectNum = hitObjectsPos.size();

	//�������Ă邩�m�F
	for (int y = 0; y < nodesYNum; y++)
	{
		for (int x = 0; x < nodesXNum; x++)
		{

			for (int i = 0; i < hitObjectNum; i++)
			{
				nodes[y][x].hitObjectNode =
					LibMath::RectAndRectCollision
					(
						nodes[y][x].position - nodeSize / 2,
						nodeSize,
						hitObjectsPos[i] - hitObjectsSize[i] / 2,
						hitObjectsSize[i]
					);

				//�Ԃ����Ă���break
				if (nodes[y][x].hitObjectNode)
				{
					break;
				}

			}
		}
	}
}

bool RouteSearch::GetAStarCalcResult
(
	const Vector2& startPos,
	const Vector2& endPos,
	std::vector< std::vector<AStarNode>>& nodes,
	std::vector<Vector2>& routeVector
)
{

	//��邱��
	//�u���b�N�ɗאڂ��Ă�}�X����A�����u���b�N�ɗאڂ��Ă�}�X�ֈړ����Ȃ��悤�ɂ���


	//���Z�b�g
	for (auto& n1 : nodes)
	{
		for (auto& n2 : n1)
		{
			n2.closeFlag = false;
			n2.openFlag = false;
			n2.previousNode = nullptr;
			n2.calcNum = UINT_MAX;
			n2.closeIndex = INT_MAX;
		}
	}

	//�X�^�[�g�n�_�Ɉ�ԋ߂��m�[�h�̋������i�[����ϐ�
	float startMinDistance = FLT_MAX;
	//�S�[���n�_�Ɉ�ԋ߂��m�[�h�̋������i�[����ϐ�
	float endMinDistance = FLT_MAX;

	auto nodeXArrayNum = nodes[0].size();
	auto nodeYArrayNum = nodes.size();

	//�X�^�[�g�̓Y����
	int startNodeIndexX = 0;
	int startNodeIndexY = 0;
	bool trueStartNodeHitFlag = false;

	//�m�[�h�̔z��̃S�[���n�_�̏ꏊ�������Y����
	int endNodeIndexX = 0;
	int endNodeIndexY = 0;
	bool trueEndNodeHitFlag = false;

	//�ꎞ�I��false�ɂ����Ƃ��ɖ߂�����
	auto ReturnHitObjectNode = [&]()
	{
		if (trueStartNodeHitFlag)nodes[startNodeIndexY][startNodeIndexX].hitObjectNode = true;
		if (trueEndNodeHitFlag)nodes[endNodeIndexY][endNodeIndexX].hitObjectNode = true;

	};

	for (int y = 0; y < nodeYArrayNum; y++)
	{
		for (int x = 0; x < nodeXArrayNum; x++)
		{
			float distance = 0.0f;
			distance = LibMath::CalcDistance2D(nodes[y][x].position, startPos);

			if (distance <= startMinDistance)
			{
				startMinDistance = distance;
				startNodeIndexX = x;
				startNodeIndexY = y;

			}

			distance = LibMath::CalcDistance2D(nodes[y][x].position, endPos);
			if (distance <= endMinDistance)
			{
				endMinDistance = distance;
				endNodeIndexX = x;
				endNodeIndexY = y;


			}

			//�C���f�b�N�X���
			nodes[y][x].indexX = x;
			nodes[y][x].indexY = y;
		}
	}

	if (nodes[startNodeIndexY][startNodeIndexX].hitObjectNode)
	{
		//�ꎞ�I��false
		nodes[startNodeIndexY][startNodeIndexX].hitObjectNode = false;
		trueStartNodeHitFlag = true;
	}

	if (nodes[endNodeIndexY][endNodeIndexX].hitObjectNode)
	{
		//�ꎞ�I��false
		nodes[endNodeIndexY][endNodeIndexX].hitObjectNode = false;
		trueEndNodeHitFlag = true;
	}

	//�S�[���̃m�[�h�܂ł̋��������߂郉���_��
	auto CalcNodeDistance = []
	(
		const int startX,
		const int startY,
		const int endX,
		const int endY
		)
	{
		//X�̍�
		int startXToEndXDiff = abs(startX - endX);
		//Y�̍�
		int startYToEndYDiff = abs(startY - endY);

		//X�̍���Y�̍��̂ǂ��炪�傫�������߂�B�΂߈ړ�����̏ꍇ�A�傫���ق����ŒZ����
		if (startXToEndXDiff >= startYToEndYDiff)return startXToEndXDiff;
		return startYToEndYDiff;

	};

	//�X�^�[�g�̃m�[�h�̃C���f�b�N�X����
	int startToEndDis = CalcNodeDistance(startNodeIndexX, startNodeIndexY, endNodeIndexX, endNodeIndexY);
	nodes[startNodeIndexY][startNodeIndexX].calcNum = startToEndDis + nodes[startNodeIndexY][startNodeIndexX].cost;

	std::vector<AStarNode*>openNodes(1, &nodes[startNodeIndexY][startNodeIndexX]);
	std::vector<AStarNode*>closeNodes;


	AStarNode* endNode;


	//�X�e�b�v�̌v�Z�͎���̃m�[�h���ׂ�Ƃ��ɂ��?

	while (1)
	{
		//�T���I�����ǂ���
		bool checkEnd = false;

		//�S�[���ɂ��ǂ蒅���Ȃ��ꍇ
		if (openNodes.size() == 0)
		{
			ReturnHitObjectNode();
			return false;
		}

		//���ёւ�
		std::sort
		(
			openNodes.begin(),
			openNodes.end(),
			[]
		(
			const AStarNode* node1,
			const AStarNode* node2
			)
		{
			int num1 = node1->calcNum;
			int num2 = node2->calcNum;

			return num1 > num2;
		});

		//calcNum����ԏ��Ȃ�����擾
		AStarNode* mainNode = nullptr;
		int minCalcNum = openNodes[openNodes.size() - 1]->calcNum;
		float minDis = FLT_MAX;
		int mainNodeOpenIndex = 0;
		for (int i = openNodes.size() - 1;; i--)
		{
			float dis = LibMath::CalcDistance2D(openNodes[i]->position, nodes[startNodeIndexY][startNodeIndexX].position);
			if (minDis > dis)
			{
				minDis = dis;
				mainNode = openNodes[i];
				mainNodeOpenIndex = i;
			}
			if (openNodes[i]->calcNum != minCalcNum || i == 0)break;
		}



		//�I�[�v���ɒǉ�����m�[�h���i�[����z��
		std::vector<AStarNode*>openPushBackNode;

		for (int y = -1; y < 2; y++)
		{
			int indexY = mainNode->indexY + y;

			//Y�͈͊O�w��h�~
			if (indexY <= -1 || indexY >= nodeYArrayNum)continue;

			for (int x = -1; x < 2; x++)
			{
				int indexX = mainNode->indexX + x;

				//X�͈͊O�w��h�~
				if (indexX <= -1 || indexX >= nodeXArrayNum)continue;

				//������h�~
				if (indexX == mainNode->indexX && indexY == mainNode->indexY)continue;

				AStarNode* checkNode = &nodes[indexY][indexX];

				//�I�u�W�F�N�g�ɏd�Ȃ��Ă邩�ǂ���
				if (checkNode->hitObjectNode)
				{
					checkNode->closeFlag = true;
					continue;
				}


				//calcNum = �X�e�b�v�� + �S�[���܂ł̋��� + �R�X�g
				int calcNum = mainNode->stepNum + 1
					+ CalcNodeDistance(indexX, indexY, endNodeIndexX, endNodeIndexY)
					+ checkNode->cost;

				//�����𖞂���������
				if (checkNode->openFlag)
				{
					if (calcNum < checkNode->calcNum)
					{
						checkNode->calcNum = calcNum;
						checkNode->previousNode = mainNode;
						checkNode->stepNum = mainNode->stepNum + 1;
					}

					continue;
				}

				//�����𖞂�������Aclose����open�Ɉړ�&���
				if (checkNode->closeFlag)
				{
					if (calcNum < checkNode->calcNum)
					{
						//close����폜
						closeNodes.erase(closeNodes.begin() + checkNode->closeIndex);
						checkNode->closeFlag = false;
						checkNode->closeIndex = INT_MAX;

						//open�ɒǉ�
						openNodes.push_back(checkNode);
						checkNode->openFlag = true;

						checkNode->calcNum = calcNum;
						checkNode->previousNode = mainNode;
						checkNode->stepNum = mainNode->stepNum + 1;
					}
					continue;
				}


				checkNode->calcNum = calcNum;
				checkNode->previousNode = mainNode;
				checkNode->stepNum = mainNode->stepNum + 1;

				if (checkNode->indexX == endNodeIndexX && checkNode->indexY == endNodeIndexY)
				{
					checkEnd = true;
					endNode = checkNode;
				}


				openPushBackNode.push_back(checkNode);
				checkNode->openFlag = true;
			}
		}

		if (checkEnd)break;

		closeNodes.push_back(mainNode);
		mainNode->closeFlag = true;
		mainNode->closeIndex = closeNodes.size() - 1;

		//���������������
		openNodes[openNodes.size() - 1]->openFlag = false;
		openNodes.erase(openNodes.begin() + mainNodeOpenIndex);


		for (auto& n : openPushBackNode)
		{
			openNodes.push_back(n);
		}
		openPushBackNode.clear();
	}

	AStarNode* currentNode = endNode;
	std::vector<Vector2>routeNodeVectors;
	while (1)
	{
		if (!currentNode)break;
		routeNodeVectors.push_back(currentNode->position);
		currentNode = currentNode->previousNode;
	}
	std::reverse(routeNodeVectors.begin(), routeNodeVectors.end());

	//�x�N�g�������߂�
	routeVector.clear();
	auto routeVectorSize = routeNodeVectors.size() - 1;
	routeVector.resize(routeVectorSize);
	for (int i = 0; i < routeVectorSize; i++)
	{
		routeVector[i] = Vector2::Normalize(routeNodeVectors[i + 1] - routeNodeVectors[i]);
	}

	ReturnHitObjectNode();
	return true;
}



#pragma endregion
