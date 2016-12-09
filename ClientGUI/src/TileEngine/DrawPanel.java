package TileEngine;

import java.awt.Graphics;

import javax.swing.JPanel;

import client.GuiGame;

public class DrawPanel extends JPanel 
{
	
	private TileLayer layer;
	
	public DrawPanel(char[][] exmap, int nbRows, int nbCols)
	{
		layer = new TileLayer(exmap, nbRows, nbCols);
				
	}
	@Override public void paintComponent(Graphics g)
	{
	super.paintComponent(g);
	
	layer.DrawLayer(g);
	}
}
