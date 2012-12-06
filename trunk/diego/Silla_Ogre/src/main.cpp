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

class SimpleExample : public ExampleApplication {
  public : void createScene() {

    // 4 Patas
    SceneNode* node1 = mSceneMgr->createSceneNode("Node1");
    Entity *pata1 = mSceneMgr->createEntity("pata1", "cube.mesh");
    node1->attachObject(pata1);
    mSceneMgr->getRootSceneNode()->addChild(node1);
    node1->setPosition(0,-15,435);
    node1->setScale(1,4,1);

    SceneNode* node2 = mSceneMgr->createSceneNode("Node2");
    Entity *pata2 = mSceneMgr->createEntity("pata2", "cube.mesh");
    node2->attachObject(pata2);
    mSceneMgr->getRootSceneNode()->addChild(node2);
    node2->setPosition(-10,-15,435);
    node2->setScale(1,4,1);

    SceneNode* node3 = mSceneMgr->createSceneNode("Node3");
    Entity *pata3 = mSceneMgr->createEntity("pata3", "cube.mesh");
    node3->attachObject(pata3);
    mSceneMgr->getRootSceneNode()->addChild(node3);
    node3->setPosition(0,-15,425);
    node3->setScale(1,4,1);

    SceneNode* node4 = mSceneMgr->createSceneNode("Node4");
    Entity *pata4 = mSceneMgr->createEntity("pata4", "cube.mesh");
    node4->attachObject(pata4);
    mSceneMgr->getRootSceneNode()->addChild(node4);
    node4->setPosition(-10,-15,425);
    node4->setScale(1,4,1);

    //Espalda (2 barrotes horiz. y 2 vert.)
    SceneNode* node5 = mSceneMgr->createSceneNode("Node5");
    Entity *espalda1 = mSceneMgr->createEntity("espalda1", "cube.mesh");
    node5->attachObject(espalda1);
    mSceneMgr->getRootSceneNode()->addChild(node5);
    node5->setPosition(0,-5,425);
    node5->setScale(1,6,1);

    SceneNode* node6 = mSceneMgr->createSceneNode("Node6");
    Entity *espalda2 = mSceneMgr->createEntity("espalda2", "cube.mesh");
    node6->attachObject(espalda2);
    mSceneMgr->getRootSceneNode()->addChild(node6);
    node6->setPosition(-10,-5,425);
    node6->setScale(1,6,1);

    SceneNode* node8 = mSceneMgr->createSceneNode("Node8");
    Entity *espalda4 = mSceneMgr->createEntity("espalda4", "cube.mesh");
    node8->attachObject(espalda4);
    mSceneMgr->getRootSceneNode()->addChild(node8);
    node8->setPosition(-5,-2,425);
    node8->setScale(4,0.3,1);

    SceneNode* node10 = mSceneMgr->createSceneNode("Node10");
    Entity *espalda5 = mSceneMgr->createEntity("espalda5", "cube.mesh");
    node10->attachObject(espalda5);
    mSceneMgr->getRootSceneNode()->addChild(node10);
    node10->setPosition(-5,0,425);
    node10->setScale(4,0.3,1);

    //Base
    SceneNode* node9 = mSceneMgr->createSceneNode("Node9");
    Entity *base = mSceneMgr->createEntity("base", "cube.mesh");
    node9->attachObject(base);
    mSceneMgr->getRootSceneNode()->addChild(node9);
    node9->setPosition(-5,-11,430);
    node9->setScale(6,0.25,6);

    // Barilla
    SceneNode* node7 = mSceneMgr->createSceneNode("Node7");
    Entity *barilla = mSceneMgr->createEntity("barilla", "cube.mesh");
    node7->attachObject(barilla);
    mSceneMgr->getRootSceneNode()->addChild(node7);
    node7->setPosition(-5,-13,435);
    node7->setScale(4,0.2,1);

  }
};

int main(void) {
  SimpleExample example;
  example.go();
  
  return 0;
}
