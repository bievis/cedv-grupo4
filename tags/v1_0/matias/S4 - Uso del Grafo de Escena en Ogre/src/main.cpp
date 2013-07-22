/*********************************************************************
 * Módulo 2. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Carlos González Morcillo     Carlos.Gonzalez@uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#include <ExampleApplication.h>

const double PROFUNDIDAD = 450;

class SimpleExample : public ExampleApplication {
  public : void createScene() {
    SceneNode* nodeToma = mSceneMgr->createSceneNode("nodeToma");
    mSceneMgr->getRootSceneNode()->addChild(nodeToma);
    nodeToma->setPosition(0,0,PROFUNDIDAD);
    nodeToma->yaw(Degree(45));

    SceneNode* nodeCubo = mSceneMgr->createSceneNode("nodeCubo");
    Entity *entCubo = mSceneMgr->createEntity("entCubo", "cube.mesh");
    nodeCubo->attachObject(entCubo);
    nodeToma->addChild(nodeCubo);
    nodeCubo->setPosition(25,8,0);
    nodeCubo->setScale(8,8,8);
    nodeCubo->pitch(Degree(45));
    nodeCubo->roll(Degree(45));

    SceneNode* nodeIndiana = mSceneMgr->createSceneNode("nodeIndiana");
    nodeToma->addChild(nodeIndiana);
    nodeIndiana->setPosition(0,0,0);
    nodeCubo->roll(Degree(15));

    SceneNode* nodeCuerpo = mSceneMgr->createSceneNode("nodeCuerpo");
    Entity *entCuerpo = mSceneMgr->createEntity("entCuerpo", "cube.mesh");
    nodeCuerpo->attachObject(entCuerpo);
    nodeIndiana->addChild(nodeCuerpo);
    nodeCuerpo->setScale(1,2,1);

    SceneNode* nodeCabeza = mSceneMgr->createSceneNode("nodeCabeza");
    Entity *entCabeza = mSceneMgr->createEntity("entCabeza", "cube.mesh");
    nodeCabeza->attachObject(entCabeza);
    nodeIndiana->addChild(nodeCabeza);
    nodeCabeza->setPosition(0,3.5,0);
    nodeCabeza->roll(Degree(-15));

    SceneNode* nodeSobrero1 = mSceneMgr->createSceneNode("nodeSobrero1");
    Entity *entSobrero1 = mSceneMgr->createEntity("entSobrero1", "cube.mesh");
    nodeSobrero1->attachObject(entSobrero1);
    nodeCabeza->addChild(nodeSobrero1);
    nodeSobrero1->setScale(1.5,-0.15,1);
    nodeSobrero1->setPosition(0,2,0);
    nodeSobrero1->roll(Degree(-15));

    SceneNode* nodeSobrero2 = mSceneMgr->createSceneNode("nodeSobrero2");
    Entity *entSobrero2 = mSceneMgr->createEntity("entSobrero2", "cube.mesh");
    nodeSobrero2->attachObject(entSobrero2);
    nodeCabeza->addChild(nodeSobrero2);
    nodeSobrero2->setScale(0.75,0.5,1);
    nodeSobrero2->setPosition(0,2.65,0);
    nodeSobrero2->roll(Degree(-15));

    SceneNode* nodePiernaIzq1 = mSceneMgr->createSceneNode("nodePiernaIzq1");
    Entity *entPiernaIzq1 = mSceneMgr->createEntity("entPiernaIzq1", "cube.mesh");
    nodePiernaIzq1->attachObject(entPiernaIzq1);
    nodeIndiana->addChild(nodePiernaIzq1);
    nodePiernaIzq1->setPosition(2,-3,1.3);
    nodePiernaIzq1->setScale(0.3,1.5,0.3);
    nodePiernaIzq1->roll(Degree(60));

    SceneNode* nodePiernaIzq2 = mSceneMgr->createSceneNode("nodePiernaIzq2");
    Entity *entPiernaIzq2 = mSceneMgr->createEntity("entPiernaIzq2", "cube.mesh");
    nodePiernaIzq2->attachObject(entPiernaIzq2);
    nodePiernaIzq1->addChild(nodePiernaIzq2);
    nodePiernaIzq2->setPosition(5,-1.5,0);
    nodePiernaIzq2->roll(Degree(60));

    SceneNode* nodePiernaDer = mSceneMgr->createSceneNode("nodePiernaDer");
    Entity *entPiernaDer = mSceneMgr->createEntity("entPiernaDer", "cube.mesh");
    nodePiernaDer->attachObject(entPiernaDer);
    nodeIndiana->addChild(nodePiernaDer);
    nodePiernaDer->setPosition(-3,-3.5,-1.3);
    nodePiernaDer->setScale(0.3,3,0.3);
    nodePiernaDer->roll(Degree(-60));

    SceneNode* nodeBrazoIzq = mSceneMgr->createSceneNode("nodeBrazoIzq");
    Entity *entBrazoIzq = mSceneMgr->createEntity("entBrazoIzq", "cube.mesh");
    nodeBrazoIzq->attachObject(entBrazoIzq);
    nodeIndiana->addChild(nodeBrazoIzq);
    nodeBrazoIzq->setPosition(-2,1,1.3);
    nodeBrazoIzq->setScale(0.3,2.5,0.3);
    nodeBrazoIzq->roll(Degree(105));

    SceneNode* nodeBrazoDer = mSceneMgr->createSceneNode("nodeBrazoDer");
    Entity *entBrazoDer = mSceneMgr->createEntity("entBrazoDer", "cube.mesh");
    nodeBrazoDer->attachObject(entBrazoDer);
    nodeIndiana->addChild(nodeBrazoDer);
    nodeBrazoDer->setPosition(-2,1.5,-1.3);
    nodeBrazoDer->setScale(0.3,2.5,0.3);
    nodeBrazoDer->roll(Degree(85));
    //nodeCabeza->setScale(1,-8,1);
    /*node1->yaw(Degree(-45));
    node1->pitch(Radian(Math::PI/4.0));*/

    /*SceneNode* node2 = mSceneMgr->createSceneNode("Node2");
    Entity *ent2 = mSceneMgr->createEntity("ent2", "cuboejes.mesh");
    node2->attachObject(ent2);
    mSceneMgr->getRootSceneNode()->addChild(node2);
    node2->setPosition(-10,0,470);

    SceneNode* node3 = mSceneMgr->createSceneNode("Node3");
    Entity *ent3 = mSceneMgr->createEntity("ent3", "cuboejes.mesh");
    node3->attachObject(ent3);
    node1->addChild(node3);
    node3->setPosition(5,0,0);

    SceneNode* node4 = mSceneMgr->createSceneNode("Node4");
    Entity *ent4 = mSceneMgr->createEntity("ent4", "cuboejes.mesh");
    node4->attachObject(ent4);
    node1->addChild(node4);
    node4->setPosition(0,0,5);
    node4->yaw(Degree(-90));*/
  }
};

int main(void) {
  SimpleExample example;
  example.go();
  
  return 0;
}
