/**
 * @name Traffic Simulation.
 * @file CameraTest.cpp
 * @class CameraTest
 * @author Sándor Balázs - AZA6NL
 * @date 2021.05.01.
 * @brief GTest UnitTest file for the Camera Class.
 * Contact sandorbalazs9402@gmail.com
*/

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include "pch.h"
#include "View/Camera.h"
#include "View/Camera.cpp"

/**
 * @brief The man test name space.
*/
namespace TrafficSimulatorTest {
	/**
	 * @brief The Camera class transfer class for access the protected fields
	 * and methods for the white box testing.
	*/
	class CameraTester : public Camera {
		public:
		glm::vec3 defaultCameraPositionTEST() {
			return defaultCameraPosition;
		}
		glm::vec3 defaultLookedPointTEST() {
			return  defaultLookedPoint;
		}
		glm::vec3 defaultUpwardVectorTEST() {
			return  defaultUpwardVector;
		}
		float angleTEST() {
			return  angle;
		}
		float aspectTEST() {
			return  aspect;
		}
		float zNearTEST() {
			return  zNear;
		}
		float zFarTEST() {
			return  zFar;
		}
		void updateTEST() {
			update();
		}
		void cameraCoordinateUpdateTEST() {
			cameraCoordinateUpdate();
		}
		void sphericalCoordinateUpdateTEST() {
			sphericalCoordinateUpdate();
		}
		void sphericalCoordinateShiftTEST(float radius, float azimuth, float inclination) {
			sphericalCoordinateShift(radius, azimuth, inclination);
		}
		void setProjectionTEST(float angle, float aspect, float zNear, float zFar) {
			setProjection(angle, aspect, zNear, zFar);
		}
		glm::vec3 sphericalCameraPositionTEST() {
			return sphericalCameraPosition;
		}
		glm::mat4 viewMatrixTEST() {
			return viewMatrix;
		}
		glm::vec3 cameraPositionTEST() {
			return cameraPosition;
		}
		glm::vec3 upwardDirectionTEST() {
			return upwardDirection;
		}
		glm::vec3 lookedPointTEST() {
			return lookedPoint;
		}
		glm::mat4 projectionMatrixTEST() {
			return projectionMatrix;
		}
		std::set<int> pressedKeysTEST() {
			return pressedKeys;
		}
	};

	/**
	 * @brief The test control class for the Camera testing.
	*/
	class CameraTest : public ::testing::Test, public Camera {
	protected:
		CameraTester camera;
		CameraTest() {}
		~CameraTest() override {}
		void SetUp() override {}
		void TearDown() override {}
	};

	/**
	 * @brief Camera constructor test.
	 * \test TESTCASE 1: Camera position is updated to default Camera position?
	 * \test TESTCASE 2: Looked point is defined to zero?
	 * \test TESTCASE 3: Upward vector is updated to the default?
	 * \test TESTCASE 4: Spherical coordinates are updated to zero?
	 * The calculations correctness is not important in this test, just the calculations fact.
	 * The calculations correctness is tested in the calculator functions test.
	 * @param CameraTest The test class
	 * @param constructorTest the test name
	*/
	TEST_F(CameraTest, constructorTest) {
		//\test TESTCASE 1: Camera position is updated to default Camera position?
		EXPECT_FLOAT_EQ(camera.cameraPositionTEST().x, camera.defaultCameraPositionTEST().x);
		EXPECT_FLOAT_EQ(camera.cameraPositionTEST().y, camera.defaultCameraPositionTEST().y);
		EXPECT_FLOAT_EQ(camera.cameraPositionTEST().z, camera.defaultCameraPositionTEST().z);

		//\test TESTCASE 2: Looked point is defined to zero?
		EXPECT_TRUE(camera.lookedPointTEST() == glm::vec3(0.0f));

		//\test TESTCASE 3: Upward vector is updated to the default?
		EXPECT_TRUE(camera.upwardDirectionTEST() == camera.defaultUpwardVectorTEST());

		//\test TESTCASE 4: Spherical coordinates are updated to zero?
		EXPECT_FALSE(camera.sphericalCameraPositionTEST() == glm::vec3(0.0f));
		
		//std::cout << "cameraPos: " << camera.cameraPositionTEST().x << "," << camera.cameraPositionTEST().y << "," << camera.cameraPositionTEST().z << std::endl;
		//std::cout << "cameradEFPos: " << camera.defaultCameraPositionTEST().x << "," << camera.defaultCameraPositionTEST().y << "," << camera.defaultCameraPositionTEST().z << std::endl;
		//std::cout <<  "sphPos: " << camera.sphericalCameraPositionTEST().x << ","  << camera.sphericalCameraPositionTEST().y << "," << camera.sphericalCameraPositionTEST().z << std::endl;
	}

	/**
	 * @brief Testing setViewTest.
	 * \test TESTCASE 1: The Camera position was recalculated?
	 * \test TESTCASE 2: The spherical coordinates recalculated?
	 * \test TESTCASE 3: The looked point is updated?
	 * The calculations correctness is not important in this test, just the calculations fact.
	 * The calculations correctness is tested in the calculator functions test.
	 * @param CameraTest The test class
	 * @param setViewTest the test name
	*/
	TEST_F(CameraTest, setViewTest) {
		glm::vec3 previousCameraPosition = camera.cameraPositionTEST();
		glm::vec3 newLookedPoint = glm::vec3(33.0f, 42.0f, 2.0f);
		glm::vec3 previousSphericalPosition = camera.sphericalCameraPositionTEST();

		camera.setView(glm::vec3(45.0f, 77.0f, 36.0f), newLookedPoint);

		//\test TESTCASE 1: The Camera position was recalculated?
		EXPECT_FALSE(camera.cameraPositionTEST() == previousCameraPosition);

		//\test TESTCASE 2: The spherical coordinates recalculated ?
		EXPECT_FALSE(camera.sphericalCameraPositionTEST() == previousSphericalPosition);

		//\test TESTCASE 3: The looked point is updated ?
		EXPECT_TRUE(camera.lookedPointTEST() == newLookedPoint);

		EXPECT_FLOAT_EQ(camera.lookedPointTEST().x, newLookedPoint.x);
		EXPECT_FLOAT_EQ(camera.lookedPointTEST().y, newLookedPoint.y);
		EXPECT_FLOAT_EQ(camera.lookedPointTEST().z, newLookedPoint.z);
	}

	/**
	 * @brief Testing setCameraPosition.
	 * \test TESTCASE 1: The Camera position was recalculated?
	 * \test TESTCASE 2: The spherical coordinates recalculated?
	 * \test TESTCASE 3: The looked point is updated?
	 * The calculations correctness is not important in this test, just the calculations fact.
	 * The calculations correctness is tested in the calculator functions test.
	 * @param CameraTest The test class
	 * @param setCameraPositionTest the test name
	*/
	TEST_F(CameraTest, setCameraPositionTest) {
		glm::vec3 previousCameraPosition = camera.cameraPositionTEST();
		glm::vec3 previousLookedPoint = camera.lookedPointTEST();
		glm::vec3 previousSphericalPosition = camera.sphericalCameraPositionTEST();

		camera.setCameraPosition(glm::vec3(12.0f, 1.0f, 48.0f));

		//\test TESTCASE 1: The Camera position was recalculated ?
		EXPECT_FALSE(camera.cameraPositionTEST() == previousCameraPosition);

		//\test TESTCASE 2: The spherical coordinates recalculated ?
		EXPECT_FALSE(camera.sphericalCameraPositionTEST() == previousSphericalPosition);

		//\test TESTCASE 3: The looked point is updated ?
		EXPECT_TRUE(camera.lookedPointTEST() == previousLookedPoint);
	}

	/**
	 * @brief Testing setLookedPoint.
	 * \test TESTCASE 1: The Camera position was recalculated?
	 * \test TESTCASE 2: The spherical coordinates recalculated?
	 * \test TESTCASE 3: The looked point is updated?
	 * \test TESTCASE 4: The looked point update is correct?
	 * The calculations correctness is not important in this test, just the calculations fact.
	 * The calculations correctness is tested in the calculator functions test.
	 * @param CameraTest The test class
	 * @param setLookedPointTest the test name
	*/
	TEST_F(CameraTest, setLookedPointTest) {
		glm::vec3 previousCameraPosition = camera.cameraPositionTEST();
		glm::vec3 previousLookedPoint = camera.lookedPointTEST();
		glm::vec3 previousSphericalPosition = camera.sphericalCameraPositionTEST();

		glm::vec3 newLookedPoint = glm::vec3(2.0f, 31.0f, 12.0f);
		camera.setLookedPoint(newLookedPoint);

		//\test TESTCASE 1: The Camera position was recalculated?
		EXPECT_FALSE(camera.cameraPositionTEST() == previousCameraPosition);
		//\test TESTCASE 2: The spherical coordinates recalculated?
		EXPECT_FALSE(camera.sphericalCameraPositionTEST() == previousSphericalPosition);
		//\test TESTCASE 3: The looked point is updated?
		EXPECT_FALSE(camera.lookedPointTEST() == previousLookedPoint);
		//\test TESTCASE 4: The looked point update is correct?
		EXPECT_TRUE(camera.lookedPointTEST() == newLookedPoint);
	}

	/**
	 * @brief Testing getCameraPosition.
	 * \test TESTCASE 1: The method get the camera position?
	 * @param CameraTest The test class.
	 * @param getCameraPositionTest the test name.
	*/
	TEST_F(CameraTest, getCameraPositionTest) {
		//\test TESTCASE 1: The method get the camera position?
		EXPECT_TRUE(camera.getCameraPosition() == camera.cameraPositionTEST());
	}

	/**
	 * @brief Testing getLookedPoint.
	 * \test TESTCASE 1: The method get the looked point?
	 * @param CameraTest The test class.
	 * @param getLookedPointTest the test name.
	*/
	TEST_F(CameraTest, getLookedPointTest) {
		//\test TESTCASE 1: The method get the looked point?
		EXPECT_TRUE(camera.getLookedPoint() == camera.lookedPointTEST());
	}

	/**
	 * @brief Testing getUpwardVector.
	 * \test TESTCASE 1: The method get the upward vector?
	 * @param CameraTest The test class.
	 * @param getUpwardVectorTest the test name.
	*/
	TEST_F(CameraTest, getUpwardVectorTest) {
		//\test TESTCASE 1: The method get the upward vector?
		EXPECT_TRUE(camera.getUpwardVector() == camera.upwardDirectionTEST());
	}

	/**
	 * @brief Testing getViewMatrix.
	 * \test TESTCASE 1: The method get the view matrix?
	 * @param CameraTest The test class.
	 * @param getViewMatrixTest The test name.
	*/
	TEST_F(CameraTest, getViewMatrixTest) {
		//\test TESTCASE 1: The method get the view matrix?
		EXPECT_TRUE(camera.getViewMatrix() == camera.viewMatrixTEST());
	}

	/**
	 * @brief Testing getViewMatrix.
	 * \test TESTCASE 1: The method get the projection matrix?
	 * @param CameraTest The test class.
	 * @param getProjectionMatrixTest The test name.
	*/
	TEST_F(CameraTest, getProjectionMatrixTest) {
		//\test TESTCASE 1: The method get the projection matrix?
		EXPECT_TRUE(camera.getProjectionMatrix() == camera.projectionMatrixTEST());
	}

	/**
	 * @brief Testing getProjectionViewMatrix.
	 * \test TESTCASE 1: The method get the projection matrix ant the view matrix product?
	 * @param CameraTest The test class.
	 * @param getProjectionViewMatrixTest The test name.
	*/
	TEST_F(CameraTest, getProjectionViewMatrixTest) {
		//\test TESTCASE 1: The method get the projection matrix ant the view matrix product?
		EXPECT_TRUE(camera.getProjectionViewMatrix() == camera.projectionMatrixTEST()*camera.viewMatrixTEST());
	}




	/**
	 * @brief Testing setLookedPoint.
	 * \test TESTCASE 1: The Camera position was recalculated?
	 * @param CameraTest The test class
	 * @param setLookedPointTest the test name
	*/
	TEST_F(CameraTest, updateTest) {
		camera.setView(glm::vec3(39.0f, 12.0f, 23.0f), glm::vec3(21.0f, 42.0f, 2.0f));
		camera.updateTEST();
		glm::mat4 calculatedViewMatrix = glm::lookAt(camera.cameraPositionTEST(), camera.lookedPointTEST(), camera.upwardDirectionTEST());
		//\test TESTCASE 1: The Camera position was recalculated ?
		EXPECT_TRUE(calculatedViewMatrix == camera.viewMatrixTEST());
	}


}