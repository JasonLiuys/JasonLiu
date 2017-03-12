package triangle;

import static org.junit.Assert.*;

import org.junit.Before;
import org.junit.Test;

public class TriangleTest {

	private Triangle tri;
	@Before
	public void setUp() throws Exception {
		tri = new Triangle();
	}

	@Test
	public void testIsTriangle() {
		assertEquals("判断三角形模块异常",true,tri.isTriangle(3, 3, 5));
	}

	@Test
	public void testIsEquilateral() {
		assertEquals("判断等边三角形模块异常",true,tri.isEquilateral(3, 3, 3));
	}

	@Test
	public void testIsIsosceles() {
		assertEquals("判断等腰三角形模块异常",true,tri.isIsosceles(3, 3, 4));
	}

	@Test
	public void testIsRightTriangle() {
		assertEquals("判断直角三角形模块异常",true,tri.isRightTriangle(3, 4, 5));
	}

	@Test
	public void testTriangle() {
		tri.Triangle(3, 3, 5);
	}

}
