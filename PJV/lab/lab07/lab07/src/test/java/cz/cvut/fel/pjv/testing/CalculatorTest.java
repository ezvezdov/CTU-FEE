package cz.cvut.fel.pjv.testing;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.AfterAll;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.BeforeAll;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.function.Executable;

import static org.junit.jupiter.api.Assertions.*;

public class CalculatorTest {

    /**
     * Test of add method, of class Calculator.
     */
    @Test
    public void testAdd() {
        System.out.println("add");
        int a = 10;
        int b = 1;
        Calculator instance = new Calculator();
        int expResult = 11;
        int result = instance.add(a, b);
        assertEquals(expResult, result);
    }

    /**
     * Test of subtract method, of class Calculator.
     */
    @Test
    public void testSubtract() {
        assertEquals(15, new Calculator().subtract(20, 5));
    }

    /**
     * Test of multiply method, of class Calculator.
     */
    @Test
    public void testMultiply() {
        assertEquals(100, new Calculator().multiply(20, 5));
    }

    /**
     * Test of divide method, of class Calculator.
     */
    @Test
    public void testDivide() {
        assertEquals(4, new Calculator().divide(20, 5));
    }

    /**
     * Test of divide method, of class Calculator.
     */
    @Test
    public void testDivideByZero() {
        assertThrows(ArithmeticException.class, new Executable() {
            @Override
            public void execute() throws Throwable {
                new Calculator().divide(20, 0);
            }
        });
    }

}
