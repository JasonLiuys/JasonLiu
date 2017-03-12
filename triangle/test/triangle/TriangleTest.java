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
		assertEquals("�ж�������ģ���쳣",true,tri.isTriangle(3, 3, 5));
	}

	@Test
	public void testIsEquilateral() {
		assertEquals("�жϵȱ�������ģ���쳣",true,tri.isEquilateral(3, 3, 3));
	}

	@Test
	public void testIsIsosceles() {
		assertEquals("�жϵ���������ģ���쳣",true,tri.isIsosceles(3, 3, 4));
	}

	@Test
	public void testIsRightTriangle() {
		assertEquals("�ж�ֱ��������ģ���쳣",true,tri.isRightTriangle(3, 4, 5));
	}

	@Test
	public void testTriangle() {
		tri.Triangle(3, 3, 5);
	}

}
