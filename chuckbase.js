// ==UserScript==
// @name         Chunkbase 种子地图批量查看器
// @namespace    http://tampermonkey.net/
// @version      1.0
// @description  批量查看Chunkbase种子地图预览图
// @author       Naszt
// @match        https://www.chunkbase.com/apps/seed-map*
// @icon         https://www.chunkbase.com/favicon.ico
// @grant        none
// ==/UserScript==

(function() {
    'use strict';

    // 创建精简查看器面板
    function createViewer() {
        if (document.getElementById('cb-viewer')) return;

        const viewer = document.createElement('div');
        viewer.id = 'cb-viewer';
        viewer.style.cssText = `
            position: fixed;
            top: 70px;
            right: 10px;
            width: 180px;
            background: white;
            border: 1px solid #2196F3;
            border-radius: 5px;
            padding: 8px;
            box-shadow: 0 2px 8px rgba(0,0,0,0.1);
            z-index: 9999;
            font-family: Arial, sans-serif;
            font-size: 11px;
        `;

        viewer.innerHTML = `
            <div style="font-weight: bold; margin-bottom: 5px; color: #2196F3;">
                批量查看器
            </div>

            <div style="margin-bottom: 5px;">
                <textarea id="cb-seeds" placeholder="seed=xxx,x=xxx,z=xxx\n每行一个"style="width:100%; height:70px; padding:4px; font-size:10px; border:1px solid #ddd; border-radius:3px; resize:vertical; font-family:monospace;"></textarea>
            </div>

            <div style="margin-bottom: 5px; font-size:10px;">
                <span>延迟:</span>
                <input type="number" id="cb-speed" value="500" min="0" max="5000" style="width:70px; padding:2px; margin:0 5px; border:1px solid #ddd; border-radius:2px;">
                <span>ms</span>
            </div>

            <div style="display: flex; gap: 4px; margin-bottom: 5px;">
                <button id="cb-start" style="flex:1; padding:4px; background:#2196F3; color:white; border:none; border-radius:3px; font-size:10px; cursor:pointer;">
                    开始查看
                </button>
                <button id="cb-stop" style="flex:1; padding:4px; background:#f44336; color:white; border:none; border-radius:3px; font-size:10px; cursor:pointer;">
                    停止
                </button>
                <button id="cb-close" style="width:20px; padding:4px; background:#ccc; color:#333; border:none; border-radius:3px; font-size:10px; cursor:pointer;">
                    ×
                </button>
            </div>

            <div id="cb-status" style="font-size:9px; color:#666; padding:3px; background:#f0f8ff; border-radius:2px; min-height:12px;">
                就绪
            </div>
        `;

        document.body.appendChild(viewer);
        setupViewerEvents();
    }

    // 设置事件
    function setupViewerEvents() {
        // 关闭
        document.getElementById('cb-close').onclick = () => {
            document.getElementById('cb-viewer').style.display = 'none';
        };

        // 开始查看
        document.getElementById('cb-start').onclick = () => {
            const input = document.getElementById('cb-seeds').value.trim();
            if (!input) {
                updateStatus('请输入数据');
                return;
            }

            const lines = input.split('\n').filter(line => line.trim());
            const seedData = parseSeedData(lines);

            if (seedData.length === 0) {
                updateStatus('格式错误');
                return;
            }

            startViewing(seedData);
        };

        // 停止
        document.getElementById('cb-stop').onclick = () => {
            window.cbViewing = false;
            updateStatus('已停止');
        };
    }

    // 解析种子数据
    function parseSeedData(lines) {
        const result = [];
        for (const line of lines) {
            const match = line.match(/seed=([^,]+),x=([^,]+),z=([^,]+)/);
            if (match) {
                result.push({
                    seed: match[1],
                    x: parseInt(match[2])+16,
                    z: parseInt(match[3])-16*0
                });
            }
        }
        return result;
    }

    // 开始查看
    async function startViewing(seedData) {
        window.cbViewing = true;
        updateStatus(`开始查看 ${seedData.length} 个种子`);
        await sleep(1000);
        document.getElementById('cb-viewer').style.display = 'none';

        for (let i = 0; i < seedData.length && window.cbViewing; i++) {
            const data = seedData[i];
            updateStatus(`${i+1}/${seedData.length}: ${data.seed}`);

            // 快速导航到种子
            window.location.hash = `seed=${data.seed}&x=${data.x}&z=${data.z}`;

            // 等待指定的延迟时间
            const delay = parseFloat(document.getElementById('cb-speed').value);
            if (delay > 0 && i < seedData.length - 1) {
                await sleep(delay);
            }
        }

        document.getElementById('cb-viewer').style.display = 'block';
        if (window.cbViewing) {
            updateStatus('查看完成');
        }
    }

    // 更新状态
    function updateStatus(msg) {
        const el = document.getElementById('cb-status');
        if (el) el.textContent = msg;
    }

    // 延时函数
    function sleep(ms) {
        return new Promise(resolve => setTimeout(resolve, ms));
    }

    // 页面加载后初始化
    setTimeout(() => {
        createViewer();

        // 添加快捷键：Ctrl+Shift+V 切换查看器
        document.addEventListener('keydown', (e) => {
            if (e.ctrlKey && e.shiftKey && e.key === 'V') {
                e.preventDefault();
                const viewer = document.getElementById('cb-viewer');
                if (viewer) {
                    viewer.style.display = viewer.style.display === 'none' ? 'block' : 'none';
                }
            }
        });
    }, 1500);
})();