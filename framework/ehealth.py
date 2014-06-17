from pintaros import model,view,fields
from pintaros.engine import gui

DEBUG = 1

class DataPasienModel(model.Model):
    _name = 'ehealth.datapasien'
    _fid = 0x1234

    _columns = {
        'rekmed': fields.CharFields(20,'No Rekam Medis'),
        'kategori': fields.CharFields(50,'Kategori Pasien'),
        'asuransi': fields.CharFields(80,'No Asuransi'),
        'tanggaldaftar': fields.DateFields('Tanggal Daftar'),
        'nama' : fields.CharFields(50,'Nama'),
        'nama_kk': fields.CharFields(50,'Nama KK'),
        'hub_keluarga': fields.SelectFields(['Sendiri','Orang Tua','Kakek/Nenek','Anak','Saudara Kandung','Saudara Ayah','Saudara Ibu','Suami/Istri'],'drop','Hubungan Keluarga'),
        'kelamin' : fields.SelectFields(['Pria','Wanita'],'radio','Jenis Kelamin'),
        'alamat' : fields.CharFields(100,'Alamat'),
        'rt' : fields.NumberFields(100, 'RT'),
        'rw' : fields.NumberFields(100, 'RT'),
        'kelurahan' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Kelurahan/Desa'),
        'kecamatan' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Kecamatan'),
        'kota' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Kota/Kabupaten'),
        'propinsi' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Propinsi'),
        'kodepos': fields.CharFields(5,'Kode Pos'),
        'kebangsaan' : fields.SelectFields(['WNI', 'WNA'], 'radio', 'Kebangsaan'),
        'tempatlahir' : fields.CharFields(20,'Tempat Lahir'),
        'tanggallahir' : fields.DateFields('Tanggal Lahir'),
        'goldarah' : fields.SelectFields(['A', 'B', 'AB', 'O', ''], 'radio', 'Golongan Darah'),
        'telepon' : fields.CharFields(16,'Telepon/HP'),
        'ktp' : fields.CharFields(16,'No KTP'),
        'status' : fields.SelectFields(['Menikah','Belum Menikah','Duda/Janda'],'radio','Status Pernihakan'),
        'agama' : fields.SelectFields(['Islam','Kristen','Protestan','Hindu','Budha','Lainnya'],'drop','Agama'),
        'pekerjaan' : fields.SelectFields(['PNS','TNI/POLRI','Pensiunan','Swasta','Pedagang','Nelayan','Petani','Pekerja lepas / Wiraswasta','Ibu Rumah Tangga', 'Pelajar', 'Mahasiswa', 'Dibawah umur', 'Tidak bekerja'],'drop','Pekerjaan'),
        'pendidikan' : fields.SelectFields(['SD','SMP','SMA','D1','D2','D3','S1/D4','S2','S3'],'drop','Pendidikan'),
        'kontak_nama' : fields.CharFields(50,'Nama'),
        'kontak_hubungan' : fields.SelectFields(['Orang Tua','Kakek/Nenek','Anak','Saudara Kandung','Saudara Ayah','Saudara Ibu','Suami/Istri'],'drop','Hubungan Keluarga'),
        'kontak_alamat' : fields.CharFields(100, 'Alamat'),
        'kontak_rt' : fields.NumberFields(100, 'RT'),
        'kontak_rw' : fields.NumberFields(100, 'RT'),
        'kontak_kelurahan' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Kelurahan/Desa'),
        'kontak_kecamatan' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Kecamatan'),
        'kontak_kota' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Kota/Kabupaten'),
        'kontak_propinsi' : fields.SelectFields(['satu', 'dua', 'tiga'], 'drop', 'Propinsi'),
        'kontak_kodepos': fields.CharFields(5,'Kode Pos'),
        'kontak_telepon' : fields.CharFields(16,'Telepon/HP'),        
    }

class RekamMedikDinamikModel(model.RecordModel):
    _name = 'ehealth.rekammedik.dinamik'
    _fid = 0x4321
    _max_record = 10
    
    _columns = {
        'tanggal_periksa': fields.DateFields('Tanggal Periksa'),
        'keluhan_utama': fields.CharFields(50,'Keluhan Utama'),
        'anamnesa': fields.CharFields(200,'Anamnesa'),
        'riwayat_dahulu': fields.CharFields(100,'Riwayat Penyakit Dahulu'),
        'riwayat_kerabat': fields.CharFields(100,'Riwayat Penyakit Keluarga/Kerabat'),
        'fisik': fields.CharFields(100, 'Pemeriksaan Fisik'),
        'tinggi': fields.NumberFields(255,'Tinggi Badan'),
        'berat': fields.NumberFields(255,'Berat Badan'),
        'systole': fields.NumberFields(255,'Systole'),
        'diastole': fields.NumberFields(255,'Diastole'),
        'nadi': fields.NumberFields(255,'Nadi'),
        'kesadaran': fields.SelectFields(['Sadar', 'Tidak sadar'], 'radio', 'Kesadaran'),
        'suhu': fields.NumberFields(255,'Suhu Badan'),
        'respirasi': fields.NumberFields(255,'Respirasi'),
        'lain': fields.CharFields(250,'Lain-lain'),
        'lab_exec_flag': fields.SelectFields([],'check','Lab Execute'),
        'expert': fields.CharFields(250,'Expertise Lab/Radio/dll'),
        'catatan_lab': fields.CharFields(50,'Catatan Lab'),
        'terapi': fields.CharFields(512,'Terapi'),
        'resep': fields.CharFields(200,'Resep'),
        'catatan_resep': fields.CharFields(50,'Catatan Resep'),
        'exec_resep_flag': fields.SelectFields([],'check', 'Eksekusi Resep'),
        'repetisi_resep': fields.SelectFields([],'check','Repetisi Resep'),
        'prognosa': fields.SelectFields(['Ya','Tidak'],'radio','Prognosa'),
        'poli': fields.CharFields(50,'Poli yang dituju'),
        'rujukan': fields.CharFields(30,'Rujukan/Pengirim penderita')
    }
        

class DataPasienView(view.SimpleView):
    _name = "ID"
    _model_name = 'ehealth.datapasien'
    _columns = ['nama',
                'kelamin',
                'alamat',
                'rt',
                'rw',
                'kelurahan',
                'kecamatan',
                'kota',
                'propinsi',
                'kebangsaan',
                'tempatlahir',
                'tanggallahir',
                'goldarah',
                'telepon',
                'ktp',
                'status',
                'agama',
                'pekerjaan',
                'pendidikan',
    ]

class RekamMedikDinamikView(view.RecordView):
    _name = "Rekam Medik Dinamik"
    _model_name = 'ehealth.rekammedik.dinamik'
    _max_record = 10

    _columns = [
        'tanggal_periksa',
        'keluhan_utama',
        'anamnesa',
        'riwayat_dahulu',
        'riwayat_kerabat',
        'fisik',
        'tinggi',
        'berat',
        'systole',
        'diastole',
        'nadi',
        'kesadaran',
        'suhu',
        'respirasi',
        'lain',
        'lab_exec_flag',
        'expert',
        'catatan_lab',
        'terapi',
        'resep',
        'catatan_resep',
        'exec_resep_flag',
        'repetisi_resep',
        'prognosa',
        'poli',
        'rujukan',
    ]

    


# app_models = [DataPasienModel,RekamMedikDinamikModel]
# app_view = [DataPasienView,RekamMedikDinamikView]

# app = gui.Gui()
# app.run()
