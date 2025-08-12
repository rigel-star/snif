import { BrowserRouter, Route, Routes } from "react-router";
import "./index.css";
import HomePage from "./pages/HomePage";
import PacketListenerPage from "./pages/listener/PacketListenerPage";

export default function App() {
	return (
		<BrowserRouter>
            <Routes>
				<Route path="/" element={<HomePage />} />
                <Route path="/if" element={<PacketListenerPage />} />
            </Routes>
		</BrowserRouter>
	);
}