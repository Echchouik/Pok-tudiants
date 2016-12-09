package TileEngine;

import java.awt.BorderLayout;
import java.awt.EventQueue;
import java.awt.Frame;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;

public class Map extends JFrame {

	/**
	 * Create the frame.
	 */
	public Map(char[][] exmap,int nbRows, int nbCols) {
		getContentPane().setLayout(null);
		JFrame frame = new JFrame("Poketudiant map");
		DrawPanel panel = new DrawPanel(exmap, nbRows, nbCols);
		JFrame.setDefaultLookAndFeelDecorated(true);
		frame.setSize(600, 600);
		frame.setContentPane(panel);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.addKeyListener(new KeyListener() {

		      /** Handle the key typed event from the text field. */
		      public void keyTyped(KeyEvent e) {
		        displayInfo(e, "KEY TYPED: ");
		      }

		      /** Handle the key pressed event from the text field. */
		      public void keyPressed(KeyEvent e) {
		        displayInfo(e, "KEY PRESSED: ");
		      }

		      /** Handle the key released event from the text field. */
		      public void keyReleased(KeyEvent e) {
		        displayInfo(e, "KEY RELEASED: ");
		      }

		      protected void displayInfo(KeyEvent e, String s) {
		        String keyString;

		        // You should only rely on the key char if the event
		        // is a key typed event.
		        int id = e.getID();
		        if (id == KeyEvent.KEY_TYPED) {
		          char c = e.getKeyChar();
		          keyString = "key character = '" + c + "'";
		        } else {
		          int keyCode = e.getKeyCode();
		          keyString = "key code = " + keyCode + " (" + KeyEvent.getKeyText(keyCode) + ")";
		        }

		        System.out.println(keyString);
		      }

		    });
		frame.setVisible(true);
	}
}