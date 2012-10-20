import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.net.Socket;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JSlider;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;


public class AppWindow extends JFrame {

	JButton _btn_fwd;
	JButton _btn_bwd;
	JButton _btn_play;
	JButton _btn_pause;
	JButton _btn_stop;
	JSlider _sli_volume;
	

	SocketClient _s;
	
	public AppWindow(SocketClient s){
		_s = s;
		this.getContentPane().setLayout(null);
		this.initWindow();
		
	}
	
	private void btn_bwd_clicked(){
		_s.prevSong();
	}
	
	private void btn_fwd_clicked(){
		_s.nextSong();
	}
	
	private void btn_play_clicked(){
		_s.play();
	}
	
	private void btn_pause_clicked(){
		_s.pause();
	}
	
	private void btn_stop_clicked(){
		_s.stop();
	}
	
	private void sli_changed(int val){
		_s.changeVolume(val);
	}
	
	

	private void initWindow(){
		_btn_fwd = new JButton("FWD");
		_btn_bwd = new JButton("BWD");
		_btn_play = new JButton("PLAY");
		_btn_pause = new JButton("PAUSE");
		_btn_stop = new JButton("STOP");
		_sli_volume = new JSlider(JSlider.VERTICAL, 0, 99, 0);
		_btn_bwd.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				btn_bwd_clicked();
			}
			
		});
				
		_btn_fwd.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				btn_fwd_clicked();
			}
			
		});

		
		_btn_play.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				btn_play_clicked();
			}
			
		});

		_btn_pause.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				btn_pause_clicked();
			}
			
		});

		
		_btn_stop.addActionListener(new ActionListener() {

			public void actionPerformed(ActionEvent arg0) {
				btn_stop_clicked();
			}
			
		});
		
		_sli_volume.addChangeListener(new ChangeListener() {
			public void stateChanged(ChangeEvent e){
				int val = _sli_volume.getValue();
				sli_changed(val);
			}
		});
		this.addWindowListener(new WindowAdapter(){
		      public void windowClosing(WindowEvent e){
		        _s.close_socket();
		      }
		    });
		
		_btn_play.setBounds(10, 10, 100, 50);
		_btn_pause.setBounds(120, 10, 100, 50);
		
		_btn_bwd.setBounds(10, 70, 100, 50);
		_btn_fwd.setBounds(120, 70, 100, 50);
		
		_btn_stop.setBounds(10, 130, 210, 50);
		
		_sli_volume.setBounds(250, 10, 100, 75);
	
		
		this.getContentPane().add(_btn_bwd);
		this.getContentPane().add(_btn_fwd);
		this.getContentPane().add(_btn_play);
		this.getContentPane().add(_btn_pause);
		this.getContentPane().add(_btn_stop);
		this.getContentPane().add(_sli_volume);
		
		
		
		this.pack();
		this.setBounds(100, 100, 400, 300);
		this.setVisible(true);
		
	}
}
