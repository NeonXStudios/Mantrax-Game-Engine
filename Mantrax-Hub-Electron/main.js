const { app, BrowserWindow, Menu, ipcMain, dialog } = require('electron');
const { exec } = require('child_process');
const fs = require('fs');
const https = require('https');
const path = require('path');
const unzipper = require('unzipper');
const os = require('os');
const asar = require('asar');
const axios = require('axios');
const url_assets = 'https://mantrax.neonxstudios.com/getdata.php'; 

async function fetchAssets() {
    try {
        const response = await axios.get(url_assets);
        const data = response.data;

        return data;
    } catch (error) {
        console.error('Error al obtener los datos:', error.message);
    }
}

let mainWindow;
const configPath = path.join(app.getPath('userData'), 'config.json');
const cachePath = path.join(app.getPath('userData'), 'MantraxData/');
const programFiles = process.env['PROGRAMFILES'] || 'C:\\Program Files';
const homeDir = os.homedir();

const documentosDir = path.join(homeDir, 'Documents/MantraxProyects');

const defaultConfig = {
    theme: 'dark',
    language: 'en',
    projectPath: documentosDir,
    advancedSettings: {},
    enginePath: ''
};


function ensureDirectoryExists(dirPath) {
    try {
        if (!fs.existsSync(dirPath)) {
            fs.mkdirSync(dirPath, { recursive: true });
            console.log(`Directorio creado: ${dirPath}`);
        } else {
            console.log(`El directorio ya existe: ${dirPath}`);
        }
    } catch (error) {
        console.error(`Error al verificar/crear el directorio: ${error}`);
    }
}

ensureDirectoryExists(documentosDir);


function loadConfig() {
    try {
        if (fs.existsSync(configPath)) {
            const data = fs.readFileSync(configPath, 'utf-8');
            console.log("Config loaded");
            return JSON.parse(data);
        }
    } catch (error) {
        console.error('Error reading config file:', error);
    }

    return defaultConfig;
}

ipcMain.handle('load-config', () => {
    return loadConfig();
});

function saveConfig(config) {
    fs.writeFileSync(configPath, JSON.stringify(config, null, 2));
}

function createWindow() {
    const config = loadConfig();

    mainWindow = new BrowserWindow({
        width: 1500,
        height: 800,
        resizable: false,
        maximizable: false,
        minimizable: false,
        roundedCorners: true,
        icon: path.join(__dirname, 'assets/imgs/Logo.png'),
        frame: false,
        transparent: true,
        backgroundColor: '#00FFFFFF',
        hasShadow: true,
        webPreferences: {
            nodeIntegration: true,
            contextIsolation: false
        }
    });

    mainWindow.loadFile('index.html');
    mainWindow.setBackgroundColor('#00000000');
    Menu.setApplicationMenu(null);

    mainWindow.webContents.on('did-finish-load', () => {
        mainWindow.webContents.send('load-config', config);
    });

    //mainWindow.webContents.openDevTools();
}

app.on('closed', () => {
    mainWindow = null;
});

app.whenReady().then(() => {
    createWindow();

    app.on('activate', () => {
        if (BrowserWindow.getAllWindows().length === 0) createWindow();
    });
});

app.on('closed', () => {
    app.quit();
});

app.on('window-all-closed', () => {
    if (process.platform !== 'darwin') app.quit();
});

ipcMain.on('close-app', () => {
    app.quit();
});

ipcMain.on('minimize-app', () => {
    if (mainWindow) {
        mainWindow.minimize();
    }
});

ipcMain.on('open-engine', async (event, programPath, parameter) => {

    if (!programPath || typeof programPath !== 'string') {
        console.error('La ruta del programa no es válida.');
        return;
    }

    if (!fs.existsSync(programPath)) {
        console.error(`El archivo no existe: ${programPath}`);
        return;
    }

    const programDirectory = path.dirname(programPath);

    try {
        const command = `"${programPath}" "${parameter}"`;

        console.log(`Ejecutando: ${command} en ${programDirectory}`);

        const childProcess = exec(
            command,
            { cwd: programDirectory },
            (error, stdout, stderr) => {
                if (error) {
                    console.error(`Error al ejecutar: ${error.message}`);
                    const stackTrace = new Error().stack;
                    fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] Error al ejecutar: ${error.message}\nStacktrace:\n${stackTrace}\n`);
                }

                if (stderr) {
                    console.error(`Salida de error: ${stderr}`);
                }

                if (stdout) {
                    console.log(`Salida estándar: ${stdout}`);
                }
            }
        );

        const logFilePath = path.join(programDirectory, 'engine-crash.log');

        childProcess.stderr.on('data', (data) => {
            console.error(`Error: ${data}`);
            const stackTrace = new Error().stack;
            fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] STDERR: ${data}\nStacktrace:\n${stackTrace}\n`);
        });

        childProcess.stdout.on('data', (data) => {
            console.log(`Salida estándar: ${data}`);
            fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] STDOUT: ${data}\n`);
        });

        childProcess.on('close', (code) => {
            console.log(`Proceso finalizado con código: ${code}`);
            if (code !== 0) {
                const stackTrace = new Error().stack;
                fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] Proceso finalizó con código ${code}\nStacktrace:\n${stackTrace}\n`);
            }
        });
    } catch (err) {
        console.error(`Error al abrir el engine: ${err.message}`);
        const stackTrace = err.stack;
        const logFilePath = path.join(programDirectory, 'engine-crash.log');
        fs.appendFileSync(logFilePath, `[${new Date().toISOString()}] Error al abrir el engine: ${err.message}\nStacktrace:\n${stackTrace}\n`);
    }
});

ipcMain.on('maximize-app', () => {
    if (mainWindow) {
        if (mainWindow.isMaximized()) {
            mainWindow.restore();
        } else {
            mainWindow.maximize();
        }
    }
});


function extractZip(zipFilePath, outputDir, progressCallback) {
    return new Promise((resolve, reject) => {
        fs.mkdirSync(outputDir, { recursive: true });
        
        let totalEntries = 0;
        let extractedEntries = 0;

        fs.createReadStream(zipFilePath)
            .pipe(unzipper.Parse())
            .on('entry', (entry) => {
                totalEntries++;
                entry.autodrain();
            })
            .on('close', () => {
                console.log('Total de archivos:', totalEntries);

                // Segunda pasada: extraer archivos
                fs.createReadStream(zipFilePath)
                    .pipe(unzipper.Parse())
                    .on('entry', async (entry) => {
                        const fileName = entry.path;
                        const fullPath = path.join(outputDir, fileName);
                        console.log('Extrayendo:', fileName);
                        try {
                            if (entry.type === 'Directory') {
                                fs.mkdirSync(fullPath, { recursive: true });
                                extractedEntries++;
                                if (progressCallback) {
                                    progressCallback(Math.round((extractedEntries / totalEntries) * 100));
                                }
                                entry.autodrain();
                            } else {
                                const dirName = path.dirname(fullPath);
                                fs.mkdirSync(dirName, { recursive: true });

                                entry
                                    .pipe(fs.createWriteStream(fullPath))
                                    .on('finish', () => {
                                        extractedEntries++;
                                        if (progressCallback) {
                                            progressCallback(Math.round((extractedEntries / totalEntries) * 100));
                                        }
                                    })
                                    .on('error', (err) => {
                                        console.error('Error escribiendo archivo:', err);
                                        reject(err);
                                    });
                            }
                        } catch (err) {
                            console.error('Error procesando entrada:', err);
                            entry.autodrain();
                            reject(err);
                        }
                    })
                    .on('close', () => {
                        console.log('Extracción completada');
                        resolve();
                    })
                    .on('error', (err) => {
                        console.error('Error en la extracción:', err);
                        reject(err);
                    });
            })
            .on('error', (err) => {
                console.error('Error contando archivos:', err);
                reject(err);
            });
    });
}

ipcMain.on('download-engine', (event, downloadUrl) => {
    const downloadsPath = app.getPath('temp');
    const filePath = path.join(downloadsPath, 'Engine.zip');


    const homeDir = os.homedir();
    const drive = path.parse(homeDir).root;
    const extractPath = path.join(drive, 'MantraxEngine');

    const file = fs.createWriteStream(filePath);
    let receivedBytes = 0;
    let totalBytes = 0;
    let startTime = Date.now();

    https.get(downloadUrl, (response) => {
        if (response.statusCode !== 200) {
            event.reply('download-error', `Error en la descarga: ${response.statusCode}`);
            return;
        }

        totalBytes = parseInt(response.headers['content-length'], 10);

        response.on('data', (chunk) => {
            receivedBytes += chunk.length;
            const elapsedTime = (Date.now() - startTime) / 1000;
            const speed = (receivedBytes / elapsedTime) / 1024;

            event.reply('download-progress', {
                progress: Math.round((receivedBytes / totalBytes) * 100),
                downloaded: receivedBytes,
                total: totalBytes,
                speed: speed.toFixed(2)
            });
        });

        response.pipe(file);

        file.on('close', async () => {
            try {
                event.reply('extracting-start');

                await extractZip(filePath, extractPath, (progress) => {
                    event.reply('extract-progress', progress);
                });

                const normalizedPath = extractPath.replace(/\\/g, '/');
                event.reply('download-complete', `${normalizedPath}/Mantrax_Engine.exe`);
            } catch (err) {
                event.reply('download-error', `Error al extraer: ${err.message}`);
            }
        });
    }).on('error', (err) => {
        fs.unlink(filePath, () => { });
        event.reply('download-error', `Error en la descarga: ${err.message}`);
    });
});


ipcMain.handle('select-folder', async () => {
    const result = await dialog.showOpenDialog({
        properties: ['openDirectory'],
        title: 'Select Project Folder'
    });

    if (!result.canceled && result.filePaths.length > 0) {
        return result.filePaths[0];
    }
    return null;
});

ipcMain.handle('select-engine', async () => {
    const result = await dialog.showOpenDialog({
        properties: ['openFile'],
        title: 'Select Engine',
        filters: [
            { name: 'Executable Files', extensions: ['exe'] },
        ],
    });

    if (!result.canceled && result.filePaths.length > 0) {
        return result.filePaths[0];
    }
    return null;
});

ipcMain.on('open-dir', async (event, ruta) => {
    console.log(`Try opening folder from: ${ruta}`);

    fs.access(ruta, fs.constants.F_OK, (err) => {
        if (err) {
            console.error(`La ruta no existe: ${ruta}`);
            event.reply('open-dir-error', `La ruta no existe: ${ruta}`);
        } else {
            console.log(`Ruta válida: ${ruta}`);

            const rutaNormalizada = process.platform === 'win32' ? path.resolve(ruta).replace(/\//g, '\\') : ruta;

            let comando;

            if (process.platform === 'win32') {
                comando = `explorer "${rutaNormalizada}"`;
            } else if (process.platform === 'darwin') {
                comando = `open "${rutaNormalizada}"`;
            } else {
                comando = `xdg-open "${rutaNormalizada}"`;
            }

            exec(comando, (error) => {
                if (error) {
                    console.error(`Error abriendo la carpeta: ${error.message}`);
                    event.reply('open-dir-error', `Error abriendo la carpeta: ${error.message}`);
                } else {
                    console.log(`Carpeta abierta exitosamente: ${rutaNormalizada}`);
                    event.reply('open-dir-success', `Carpeta abierta exitosamente: ${rutaNormalizada}`);
                }
            });
        }
    });
});

ipcMain.on('save-config', (event, config) => {
    saveConfig(config);
});


ipcMain.on('create-project', async (event, data) => {
    const { projectName, includeAssets, projectPath } = data;

    console.log(`Proyecto: ${projectName}, Incluir Assets: ${includeAssets}, Ruta: ${projectPath}`);

    const pathToCreate = path.join(projectPath, projectName);

    try {
        await fs.promises.mkdir(pathToCreate, { recursive: true });

        if (includeAssets) {
            event.reply('extracting-start');

            await extractZip(`data/DefaultAssets.zip`, pathToCreate, (progress) => {
                event.reply('extract-progress', progress);
            });

            event.reply('extract-complete', pathToCreate);
        }

        event.sender.send('create-project-success', { message: 'Proyecto creado exitosamente' });
    } catch (err) {
        console.error('Error durante la creación del proyecto:', err);
        event.sender.send('create-project-error', { error: `Error: ${err.message}` });
    }
});


ipcMain.handle('load-folders', async (event, projectPath) => {
    try {
        const folders = fs.readdirSync(projectPath, { withFileTypes: true })
            .filter((dirent) => dirent.isDirectory())
            .map((dirent) => dirent.name);

        console.log('Folders found:', folders);
        return folders;
    } catch (error) {
        console.error('Error loading folders:', error);
        throw new Error('Could not load folders.');
    }
});




ipcMain.on('delete-project', async (event, projectPath) => {
    try {
        if (!fs.existsSync(projectPath)) {
            throw new Error('La carpeta no existe.');
        }

        const files = await getAllFiles(projectPath);
        const totalFiles = files.length;
        let deletedFiles = 0;

        for (const file of files) {
            await deleteFileOrFolder(file);
            deletedFiles++;
            const progress = Math.round((deletedFiles / totalFiles) * 100);
            event.reply('delete-progress', progress);
        }

        await fs.promises.rm(projectPath, { recursive: true, force: true });

        event.reply('delete-complete');
    } catch (err) {
        console.error('Error on try delete:', err);
        event.reply('delete-error', err.message);
    }
});

async function getAllFiles(dirPath) {
    let files = [];
    const items = await fs.promises.readdir(dirPath, { withFileTypes: true });

    for (const item of items) {
        const fullPath = path.join(dirPath, item.name);
        if (item.isDirectory()) {
            files = files.concat(await getAllFiles(fullPath));
        } else {
            files.push(fullPath);
        }
    }

    return files;
}

async function deleteFileOrFolder(filePath) {
    const stats = await fs.promises.lstat(filePath);

    if (stats.isDirectory()) {
        await fs.promises.rm(filePath, { recursive: true, force: true });
    } else {
        await fs.promises.unlink(filePath);
    }
}


ipcMain.on('get-project', async (event, projectPath) => {
    try {
        const data = await fetchAssets(); 
        event.reply('get-progress', data); 
    } catch (error) {
        console.error('Error al obtener los datos:', error.message);
        event.reply('delete-progress', { error: error.message }); 
    }
});

async function fetchAssets() {
    try {
        const response = await axios.get(url_assets);
        return response.data; 
    } catch (error) {
        console.error('Error al obtener los datos:', error.message);
        throw error; 
    }
}

ipcMain.on('start-download', async (event, { url, imglink, index, id }) => {
    console.log('Download started', { url, imglink, index, id }); // Verifica si el evento se recibe

    try {
        // Ruta de la carpeta "storecache"
        const storeCachePath = path.join(__dirname, 'storecache');

        // Verificar si la carpeta "storecache" existe, si no, crearla
        if (!fs.existsSync(storeCachePath)) {
            fs.mkdirSync(storeCachePath, { recursive: true }); // Asegura que se cree toda la estructura de carpetas
        }

        // Crear una carpeta aleatoria dentro de "storecache"
        const randomFolderName = id;
        const folderPath = path.join(storeCachePath, randomFolderName);
        
        if (!fs.existsSync(folderPath)) {
            fs.mkdirSync(folderPath, { recursive: true });
        }

        // Ruta para el archivo descargado
        const filePath = path.join(folderPath, `downloaded-asset-${index}.zip`);

        // Ruta para la imagen (portview.png)
        const imageFilePath = path.join(folderPath, 'portview.png');

        // Descargar la imagen
        await downloadImage(imglink, imageFilePath);

        const writer = fs.createWriteStream(filePath);
        const response = await axios({
            method: 'get',
            url,
            responseType: 'stream',
        });

        const totalLength = parseInt(response.headers['content-length'], 10);
        let downloadedLength = 0;

        response.data.on('data', (chunk) => {
            downloadedLength += chunk.length;
            const progress = Math.floor((downloadedLength / totalLength) * 100);
            console.log(`Progress: ${progress}%`); // Verifica el progreso de la descarga

            // Emitir progreso al renderer
            event.sender.send('download-progress', { index, progress });
        });

        response.data.pipe(writer);

        writer.on('finish', () => {
            event.sender.send('download-complete', { index, folderPath });
        });

        writer.on('error', (err) => {
            console.error('Error al escribir el archivo:', err);
            event.sender.send('download-error', { index, error: err.message });
        });
    } catch (error) {
        console.error('Error durante la descarga:', error.message);
        event.sender.send('download-error', { index, error: error.message });
    }
});

ipcMain.on('check-asset-existence', async (event, { asset_id }) => {
    const storeCachePath = path.join(__dirname, 'storecache');
    const folderPath = path.join(storeCachePath, asset_id);
    console.log(folderPath);

    if (fs.existsSync(folderPath)) {
        event.sender.send('asset-installed', { asset_id });
    } else {
        event.sender.send('asset-exists', { asset_id });
    }
});

// Función de descarga de imagen
async function downloadImage(imgUrl, imageFilePath) {
    try {
        const response = await axios({
            method: 'get',
            url: imgUrl,
            responseType: 'stream',
        });

        const writer = fs.createWriteStream(imageFilePath);
        response.data.pipe(writer);


        writer.on('finish', () => {
            console.log(`Imagen descargada en ${imageFilePath}`);
        });

        writer.on('error', (err) => {
            console.error('Error al descargar la imagen:', err);
        });
    } catch (error) {
        console.error('Error al descargar la imagen:', error.message);
    }
}