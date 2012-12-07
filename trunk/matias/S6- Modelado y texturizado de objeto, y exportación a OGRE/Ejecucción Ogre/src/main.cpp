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
    Ogre::Entity *ent = mSceneMgr->createEntity("Tanque", "Tanque.mesh");
    mSceneMgr->getRootSceneNode()->attachObject(ent);
  }
};

int main(void) {
  SimpleExample example;
  example.go();
  
  return 0;
}
