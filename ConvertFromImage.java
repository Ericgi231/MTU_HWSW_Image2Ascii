// Used to generate an RGB text from a binary image file

import java.awt.*;

public class ConvertFromImage
{
    public static void main(String[] args)
    {
        if (args.length == 0)
        {
            System.out.println("ConvertFromImage <image filename>");
            return;
        }
        
        Picture pic = new Picture(args[0]);

        System.out.printf("%d %d\n", pic.height(), pic.width());
        for (int y = 0; y < pic.height(); y++)
        {
            for (int x = 0; x < pic.width(); x++)
            {
                Color c = pic.get(x, y);
                int r = c.getRed();
                int g = c.getGreen();
                int b = c.getBlue();
                
                System.out.printf("%3d %3d %3d  ", r, g, b);                
            }
            System.out.println();
        }        
    }
}
