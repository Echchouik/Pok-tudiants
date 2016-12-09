package TileEngine;

import java.awt.Graphics;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class TileLayer {
	
	private char[][] map;
	private BufferedImage tileSheet;
	int nbRows;
	int nbCols;
	
	public TileLayer(char[][] existingMap, int nbRows, int nbCols)
	{
		this.nbRows = nbRows;
		this.nbCols = nbCols;
		map=new char[nbRows][nbCols];
		for (int y = 0; y < nbRows; y++) {
			for (int x = 0; x < nbCols; x++) {
				switch (existingMap[y][x]) {
				case '+':
					map[y][x] = 24;
					break;
				case '*':
					map[y][x] = 8;
					break;
				case '0':
					map[y][x] = 0;
					break;

				default:
					map[y][x] = 1;
					break;
				}
				
			}
			
		}
		tileSheet = LoadTileSheet("default.jpg");
		
	}
	
	public BufferedImage LoadTileSheet(String fileName)
	{
		BufferedImage img = null;
		try
		{
			img = ImageIO.read(new File(fileName));
			
		}
		catch (IOException e)
		{
			System.out.println("Could not load image!");
			
		}
		return img;
	}
	
	public void DrawLayer(Graphics g)
	{
		for (int y = 0; y < nbRows; y++) {
			for (int x = 0; x < nbCols; x++) {
				int index = map[y][x];
				int yOffset=0;
				if(index>(tileSheet.getWidth()/Engine.TILE_WIDTH)-1)
				{
					yOffset++;
					index=index-(tileSheet.getWidth()/Engine.TILE_WIDTH);
					
				}
				g.drawImage(tileSheet, x * Engine.TILE_WIDTH,
						y * Engine.TILE_HEIGHT,
						(x * Engine.TILE_WIDTH) + Engine.TILE_WIDTH,
						(y*Engine.TILE_HEIGHT) + Engine.TILE_HEIGHT,
						index * Engine.TILE_WIDTH,
						yOffset * Engine.TILE_HEIGHT,
						(index * Engine.TILE_WIDTH)+ Engine.TILE_WIDTH,
						(yOffset * Engine.TILE_HEIGHT) + Engine.TILE_HEIGHT,null);
				
			}
			
		}
	}

}
