package main

import (
	"fmt"
	"math"
	"math/rand"
	"time"
)

const (
	NUM_RIGID_BODIES = 1
)

type Vector2 struct {
	x float32
	y float32
}

type BoxShape struct {
	width           float32
	height          float32
	mass            float32
	momentOfInertia float32
}

func (boxShape *BoxShape) CalculateBoxInertia() {
	var (
		m float32 = boxShape.mass
		w float32 = boxShape.width
		h float32 = boxShape.height
	)
	boxShape.momentOfInertia = m * (w * w * h * h) / 12
}

type RigidBody struct {
	position        Vector2
	linearVelocity  Vector2
	angle           float32
	angularVelocity float32
	force           Vector2
	torque          float32
	shape           BoxShape
}

func (rigidBody *RigidBody) ComputeForceAndTorque() {
	var f Vector2 = Vector2{0, 100}
	rigidBody.force = f
	var r Vector2 = Vector2{
		x: rigidBody.shape.width / 2,
		y: rigidBody.shape.height / 2,
	}

	rigidBody.torque = r.x*f.y - r.y*f.x // `Cross-product`: τ = r x F
}

var rigidBodies [NUM_RIGID_BODIES]RigidBody

func main() {
	runRigidBodySimulation()
}

func printRigidBodies() {
	for i := 0; i < NUM_RIGID_BODIES; i++ {
		var rigidBody *RigidBody = &rigidBodies[i]
		fmt.Printf("body[%d] p = (%.2f, %.2f), a = %.2f\n",
			i, rigidBody.position.x, rigidBody.position.y, rigidBody.angle)
	}
}

func initializeRigidBodies() {
	rand.Seed(time.Now().UnixNano())
	for i := 0; i < NUM_RIGID_BODIES; i++ {
		var rigidBody *RigidBody = &rigidBodies[i]
		rigidBody.position = Vector2{
			x: float32(rand.Intn(50)) + rand.Float32(),
			y: float32(rand.Intn(50)) + rand.Float32(),
		}

		//rand.Seed(time.Now().UnixNano())
		rigidBody.angle = (float32(rand.Intn(360)) + rand.Float32()) / 360.0 * math.Pi * 2
		rigidBody.linearVelocity = Vector2{0, 0}
		rigidBody.angularVelocity = 0.0

		//rand.Seed(time.Now().UnixNano())
		var shape BoxShape
		shape.mass = 10
		shape.width = 1 + float32(rand.Intn(2)) + rand.Float32()
		shape.height = 1 + float32(rand.Intn(2)) + rand.Float32()
		shape.CalculateBoxInertia()
		rigidBody.shape = shape
	}
}

func runRigidBodySimulation() {
	var (
		totalSimulationTime float32 = 10.0
		currentTime         float32 = 0.0
		dt                  float32 = 1.0
	)

	initializeRigidBodies()
	printRigidBodies()

	for currentTime < totalSimulationTime {
		time.Sleep(time.Duration(dt) * time.Second)

		for i := 0; i < NUM_RIGID_BODIES; i++ {
			var rigidBody *RigidBody = &rigidBodies[i]
			rigidBody.ComputeForceAndTorque()
			var linearAcceleration Vector2 = Vector2{
				x: rigidBody.force.x / rigidBody.shape.mass,
				y: rigidBody.force.y / rigidBody.shape.mass,
			}

			rigidBody.linearVelocity.x += linearAcceleration.x * dt
			rigidBody.linearVelocity.y += linearAcceleration.y * dt
			rigidBody.position.x += rigidBody.linearVelocity.x * dt
			rigidBody.position.y += rigidBody.linearVelocity.y * dt
			var angularAcceleration float32 = rigidBody.torque / rigidBody.shape.momentOfInertia
			rigidBody.angularVelocity += angularAcceleration * dt
			rigidBody.angle += rigidBody.angularVelocity * dt
		}

		printRigidBodies()
		currentTime += dt
	}
}
